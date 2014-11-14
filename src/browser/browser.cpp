#include "browser.h"

#include "util.h"

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

	const CefSettings& GetAppSettings()
	{
		static CefSettings as;
		as.single_process = true;
		as.no_sandbox = true;
		as.multi_threaded_message_loop = false;
		as.command_line_args_disabled = true;
		as.remote_debugging_port = 8000;
		CefString(&as.cache_path) = "ui_cache";
		return as;
	}

	int generate_ipc_function_id()
	{
		static int i = 0;
		return i++;
	}

	std::string create_placeholder(int id)
	{
		return Browser::Util::get_ipc_function_string_placeholder() + std::to_string(id);
	}
}
	

std::shared_ptr<Browser::Browser> Browser::Browser::_instance(nullptr);

Browser::Browser::Browser(const PaintCallbackFunction& onPaint)
:_handler(new Handler(*this)), _onPaint(onPaint)
{
	CefMainArgs args(GetModuleHandle(nullptr));
	_application = new Application(std::bind(&Browser::on_context_created, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	int exitCode = CefExecuteProcess(args, _application.get(), nullptr);
	if (exitCode >= 0) {
		exit(exitCode);
	}

	if (!CefInitialize(args, GetAppSettings(), _application.get(), nullptr)) {
		throw std::runtime_error("Failed to initialize cef.");
	}

	_browser = CefBrowserHost::CreateBrowserSync(GetWindowInfo(), _handler.get(), "", GetBrowserSettings(), nullptr);
}

std::shared_ptr<Browser::Browser> Browser::Browser::make(const PaintCallbackFunction& onPaint)
{
	if (_instance) {
		throw std::runtime_error("Only one browser instance can be instantiated at a time.");
	}
	return _instance = std::shared_ptr<Browser>(new Browser(onPaint));
}

Browser::Browser::~Browser()
{
	_browser->GetHost()->CloseBrowser(true);
	CefShutdown();
}

void Browser::Browser::load_url(const std::string& url)
{
	_browser->GetMainFrame()->LoadURL(url);
}

void Browser::Browser::update_viewport_size(const RectSize& viewportSize)
{
	_viewportRect = CefRect(0, 0, viewportSize.width, viewportSize.height);
}

void Browser::Browser::register_api(const JSValue& api)
{
	CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create(Util::get_register_api_message_name());
	message->GetArgumentList()->SetDictionary(0, to_ipc_message_arguments_helper(static_cast<const JSValue::Object&>(api)));
	_browser->SendProcessMessage(PID_RENDERER, message);
}

void Browser::Browser::forward_key_event(Input::Pressable key, Input::PressableState state, int modifiers)
{
	CefKeyEvent ev;
	ev.type = state == Input::PressableState::UP ? KEYEVENT_KEYUP : KEYEVENT_KEYDOWN;
	ev.windows_key_code = static_cast<int>(key);;
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

void Browser::Browser::tick()
{
	CefDoMessageLoopWork();
}

Browser::Browser::Handler::Handler(Browser& browser)
:_browser(browser)
{}

CefRefPtr<CefRenderHandler> Browser::Browser::Handler::GetRenderHandler()
{
	return this;
}

bool Browser::Browser::Handler::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message)
{
	CefRefPtr<CefListValue> args = message->GetArgumentList();

	try {
		CefRefPtr<CefProcessMessage> responseMessage = CefProcessMessage::Create(Util::get_resolve_promise_message_name());
		auto returnArgs = _browser.to_ipc_message_arguments_helper(_browser._idToIPCFunction[args->GetInt(0)](JSValue::Array()));
		responseMessage->GetArgumentList()->SetList(0, returnArgs);
		responseMessage->GetArgumentList()->SetInt(1, args->GetInt(1));
		browser->SendProcessMessage(PID_RENDERER, responseMessage);
	}
	catch (std::exception& ex) {
		CefRefPtr<CefProcessMessage> responseMessage = CefProcessMessage::Create(Util::get_reject_promise_message_name());
		responseMessage->GetArgumentList()->SetString(0, ex.what());
		responseMessage->GetArgumentList()->SetInt(1, args->GetInt(1));
		browser->SendProcessMessage(PID_RENDERER, responseMessage);
	}

	return true;
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

void Browser::Browser::on_context_created(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
{
	_context = context;
}

CefRefPtr<CefListValue> Browser::Browser::to_ipc_message_arguments_helper(const JSValue::Array& o)
{
	CefRefPtr<CefListValue> out = CefListValue::Create();
	out->SetSize(o.size());
	for (size_t i = 0; i < o.size(); ++i) {
		const JSValue& val = o[i];
		if (val.is_array()) {
			out->SetList(i, to_ipc_message_arguments_helper(static_cast<const JSValue::Array&>(val)));
		}
		else if (val.is_object()) {
			out->SetDictionary(i, to_ipc_message_arguments_helper(static_cast<const JSValue::Object&>(val)));
		}
		else if (val.is_bool()) {
			out->SetBool(i, static_cast<bool>(val));
		}
		else if (val.is_null()) {
			out->SetNull(i);
		}
		else if (val.is_string()) {
			const std::string& temp = static_cast<const std::string&>(val);
			if (temp == Util::get_ipc_function_string_placeholder()) {
				throw std::runtime_error("String value cannot be " + Util::get_ipc_function_string_placeholder());
			}
			out->SetString(i, temp);
		}
		else if (val.is_function()) {
			const int id = generate_ipc_function_id();
			_idToIPCFunction[id] = static_cast<const JSValue::Function&>(val);
			out->SetString(i, create_placeholder(id));
		}
		else {
			throw std::runtime_error("Type not supported.");
		}
	}

	return out;
}

CefRefPtr<CefDictionaryValue> Browser::Browser::to_ipc_message_arguments_helper(const JSValue::Object& o)
{
	CefRefPtr<CefDictionaryValue> out = CefDictionaryValue::Create();
	std::for_each(std::begin(o), std::end(o), [this, &out](const std::pair<std::string, JSValue>& kvp){
		const std::string& key = kvp.first;
		const JSValue& val = kvp.second;

		if (val.is_array()) {
			out->SetList(key, to_ipc_message_arguments_helper(static_cast<const JSValue::Array&>(val)));
		}
		else if (val.is_object()) {
			out->SetDictionary(key, to_ipc_message_arguments_helper(static_cast<const JSValue::Object&>(val)));
		}
		else if (val.is_bool()) {
			out->SetBool(key, static_cast<bool>(val));
		}
		else if (val.is_null()) {
			out->SetNull(key);
		}
		else if (val.is_string()) {
			const std::string& temp = static_cast<const std::string&>(val);
			if (temp == Util::get_ipc_function_string_placeholder()) {
				throw std::runtime_error("String value cannot be " + Util::get_ipc_function_string_placeholder());
			}
			out->SetString(key, temp);
		}
		else if (val.is_function()) {
			const int id = generate_ipc_function_id();
			_idToIPCFunction[id] = static_cast<const JSValue::Function&>(val);
			out->SetString(key, create_placeholder(id));
		}
		else {
			throw std::runtime_error("Type not supported.");
		}
	});

	return out;
}
