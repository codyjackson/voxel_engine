#include "browser.h"

namespace
{

	const CefWindowInfo& GetWindowInfo()
	{
		static CefWindowInfo wi;
		wi.SetAsOffScreen(nullptr);
		wi.SetTransparentPainting(true);

		return wi;
	}

	const CefBrowserSettings& GetBrowserSettings()
	{
		static CefBrowserSettings bs;
		return bs;
	}

	class MyV8Handler : public CefV8Handler {
	public:
		MyV8Handler(const std::function<void(CefRefPtr<CefV8Value>, const CefV8ValueList&)>& fn) 
			:_fn(fn)
		{}

		virtual bool Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception) OVERRIDE 
		{
			try {
				_fn(object, arguments);
				return true;
			} catch (std::exception ex) {
				exception = ex.what();
				return false;
			}
		}

			// Provide the reference counting implementation for this class.
		IMPLEMENT_REFCOUNTING(MyV8Handler);

	private:
		std::function<void(CefRefPtr<CefV8Value>, const CefV8ValueList&)> _fn;
	};

	void to_js_value_helper(CefRefPtr<CefV8Value> val, JSValue& out)
	{
		if (val->IsArray()) {
			const int size = val->GetArrayLength();
			JSValue::Array a;
			a.reserve(size);
			for (int i = 0; i < size; ++i) {
				JSValue subVal;
				to_js_value_helper(val->GetValue(i), subVal);
			}
			out = a;
		} else if (val->IsObject()) {
			JSValue::Object o;
			std::vector<CefString> keys;
			val->GetKeys(keys);
			std::for_each(std::begin(keys), std::end(keys), [&o, &val](const CefString& key){
				to_js_value_helper(val->GetValue(key), o[key.ToString()]);
			});
			out = o;
		} else if (val->IsBool()) {
			out = JSValue(val->GetBoolValue());
		} else if (val->IsDouble()) {
			out = JSValue(val->GetDoubleValue());
		} else if (val->IsNull()) {
			out = JSValue::Null();
		} else if (val->IsString()) {
			out = JSValue(val->GetStringValue().ToString());
		} else {
			throw std::runtime_error("Type not supported.");
		}
	}

	JSValue to_js_value(CefRefPtr<CefV8Value> val)
	{
		JSValue out;
		to_js_value_helper(val, out);
		return out;
	}

	CefRefPtr<CefV8Value> to_cef_v8_function(const JSValue::Function& fn);
	void to_cef_v8_value_helper(const JSValue& val, CefRefPtr<CefV8Value>& out)
	{
		if (val.is_array()) {
			const JSValue::Array& temp = static_cast<const JSValue::Array&>(val);
			CefRefPtr<CefV8Value> a = CefV8Value::CreateArray(temp.size());
			for (size_t i = 0; i < temp.size(); ++i) {
				CefRefPtr<CefV8Value> subValue;
				to_cef_v8_value_helper(val[i], subValue);
				a->SetValue(i, subValue);
			}
			out = a;
		} else if (val.is_object()) {
			const JSValue::Object& temp = static_cast<const JSValue::Object&>(val);
			CefRefPtr<CefV8Value> o = CefV8Value::CreateObject(NULL);
			std::for_each(std::begin(temp), std::end(temp), [&val, &o](const JSValue::Object::value_type& kvp){
				CefRefPtr<CefV8Value> subValue;
				const std::string& key = kvp.first;
				to_cef_v8_value_helper(val[key], subValue);
				o->SetValue(key, subValue, V8_PROPERTY_ATTRIBUTE_NONE);
			});
		} else if (val.is_bool()) {
			out = CefV8Value::CreateBool(static_cast<bool>(val));
		} else if (val.is_null()) {
			out = CefV8Value::CreateNull();
		} else if (val.is_string()) {
			out = CefV8Value::CreateString(static_cast<std::string>(val));
		} else if (val.is_function()) {
			out = to_cef_v8_function(val);
		} else {
			throw std::runtime_error("Type not supported.");
		}
	}

	CefRefPtr<CefV8Value> to_cef_v8_value(const JSValue& val)
	{
		CefRefPtr<CefV8Value> out;
		to_cef_v8_value_helper(val, out);
		return out;
	}

	CefRefPtr<CefV8Value> to_cef_v8_function(const JSValue::Function& fn)
	{
		auto wrappedFn = [&fn](CefRefPtr<CefV8Value> rawObject, const CefV8ValueList& rawArguments){
			JSValue object = to_js_value(rawObject);
			JSValue::Array arguments;
			arguments.reserve(rawArguments.size());
			std::for_each(std::begin(rawArguments), std::end(rawArguments), [&arguments](CefRefPtr<CefV8Value> arg){
				arguments.push_back(to_js_value(arg));
			});
		};

		static CefString nameNotNeeded("");
		return CefV8Value::CreateFunction(nameNotNeeded, new MyV8Handler(wrappedFn));
	}
}
	

Browser::Browser::Browser(const JSValue& api, const PaintCallbackFunction& onPaint)
:_handler(new Handler(*this)), _browser(CefBrowserHost::CreateBrowserSync(GetWindowInfo(), _handler.get(), "", GetBrowserSettings(), nullptr)), _onPaint(onPaint)
{}

Browser::Browser::~Browser()
{
	_browser->GetHost()->CloseBrowser(true);
}

void Browser::Browser::load_url(const std::string& url)
{
	_browser->GetMainFrame()->LoadURL(url);
}

void Browser::Browser::update_viewport_size(const RectSize& viewportSize)
{
	_viewportRect = CefRect(0, 0, viewportSize.width, viewportSize.height);
}

void Browser::Browser::forward_key_event(Input::Pressable key, Input::PressableState state, int modifiers)
{
	CefKeyEvent ev;
	ev.character = 0;
	ev.unmodified_character = 0;
	ev.native_key_code = static_cast<int>(key);
	ev.modifiers = modifiers;
	ev.type = state == Input::PressableState::UP ? KEYEVENT_KEYUP : KEYEVENT_KEYDOWN;
	_browser->GetHost()->SendKeyEvent(ev);
}

void Browser::Browser::forward_mouse_button_event(Input::Pressable button, Input::PressableState state, int modifiers)
{
	CefBrowserHost::MouseButtonType b;
	if (button == Input::Pressable::MOUSE_BUTTON_1) {
		b = CefBrowserHost::MouseButtonType::MBT_LEFT;
	} else if (button == Input::Pressable::MOUSE_BUTTON_2) {
		b = CefBrowserHost::MouseButtonType::MBT_RIGHT;
	} else if (button == Input::Pressable::MOUSE_BUTTON_3) {
		b = CefBrowserHost::MouseButtonType::MBT_MIDDLE;
	}

	const bool isButtonUp = state == Input::PressableState::UP;
	_mouseEvent.modifiers = modifiers;
	_browser->GetHost()->SendMouseClickEvent(_mouseEvent, b, isButtonUp, 1);
}

void Browser::Browser::forward_mouse_move_event(double x, double y)
{
	_mouseEvent.x = static_cast<int>(x);
	_mouseEvent.y = static_cast<int>(y);
	_browser->GetHost()->SendMouseMoveEvent(_mouseEvent, false);
}

void Browser::Browser::forward_mouse_wheel_event(double xoffset, double yoffset)
{
	_browser->GetHost()->SendMouseWheelEvent(_mouseEvent, static_cast<int>(xoffset), static_cast<int>(yoffset));
}

const JSValue& Browser::Browser::get_api() const
{
	return _api;
}

Browser::Browser::Handler::Handler(Browser& browser)
:_browser(browser)
{}

CefRefPtr<CefRenderHandler> Browser::Browser::Handler::GetRenderHandler()
{
	return this;
}

bool Browser::Browser::Handler::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect)
{
	rect = _browser._viewportRect;
	return true;
}

void Browser::Browser::Handler::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height)
{
	_browser._onPaint(RectSize(width, height), dirtyRects, buffer);
}