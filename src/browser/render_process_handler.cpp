#include "render_process_handler.h"

#include "jsvalue.h"
#include "util.h"

namespace
{
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
			}
			catch (std::exception ex) {
				exception = ex.what();
				return false;
			}
		}

			// Provide the reference counting implementation for this class.
		IMPLEMENT_REFCOUNTING(MyV8Handler);

	private:
		std::function<void(CefRefPtr<CefV8Value>, const CefV8ValueList&)> _fn;
	};

	class ContextOpener
	{
	public:
		ContextOpener(CefRefPtr<CefV8Context> context)
			:_context(context)
		{
			_context->Enter();
		}

		~ContextOpener()
		{
			_context->Exit();
		}

		CefRefPtr<CefV8Context> get() const
		{
			return _context;
		}

	private:
		CefRefPtr<CefV8Context> _context;
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
		}
		else if (val->IsObject()) {
			JSValue::Object o;
			std::vector<CefString> keys;
			val->GetKeys(keys);
			std::for_each(std::begin(keys), std::end(keys), [&o, &val](const CefString& key){
				to_js_value_helper(val->GetValue(key), o[key.ToString()]);
			});
			out = o;
		}
		else if (val->IsBool()) {
			out = JSValue(val->GetBoolValue());
		}
		else if (val->IsDouble()) {
			out = JSValue(val->GetDoubleValue());
		}
		else if (val->IsNull()) {
			out = JSValue::Null();
		}
		else if (val->IsString()) {
			out = JSValue(val->GetStringValue().ToString());
		}
		else {
			throw std::runtime_error("Type not supported.");
		}
	}

	
	typedef std::function<void(int)> FunctionHandler;
	void to_cef_v8_value_helper(CefRefPtr<CefListValue> val, CefRefPtr<CefV8Value>& out, const FunctionHandler& functionHandler);
	void to_cef_v8_function(int id, CefRefPtr<CefV8Value>& out, const FunctionHandler& functionHandler)
	{
		out = CefV8Value::CreateFunction("anonymous", new MyV8Handler([=](CefRefPtr<CefV8Value> o, const CefV8ValueList& args){
			functionHandler(id);
		}));
	}

	void to_cef_v8_value_helper(std::string s, CefRefPtr<CefV8Value>& out, const FunctionHandler& functionHandler)
	{
		const std::string& placeholder = Browser::Util::get_ipc_function_string_placeholder();
		auto iter = std::find_first_of(std::begin(s), std::end(s), std::begin(placeholder), std::end(placeholder));
		if (iter != std::end(s)) {
			s.erase(iter, iter + placeholder.size());
			return to_cef_v8_function(std::stoi(s), out, functionHandler);
		}
		out = CefV8Value::CreateString(s);
	}

	void to_cef_v8_value_helper(CefRefPtr<CefDictionaryValue>& val, CefRefPtr<CefV8Value>& out, const FunctionHandler& functionHandler)
	{
		out = CefV8Value::CreateObject(nullptr);
		CefDictionaryValue::KeyList keys;
		val->GetKeys(keys);
		std::for_each(std::begin(keys), std::end(keys), [&](const std::string& key){
			CefRefPtr<CefV8Value> elem;
			CefValueType type = val->GetType(key);
			if (type == CefValueType::VTYPE_LIST) {
				to_cef_v8_value_helper(val->GetList(key), elem, functionHandler);
			}
			else if (type == CefValueType::VTYPE_DICTIONARY) {
				to_cef_v8_value_helper(val->GetDictionary(key), elem, functionHandler);
			}
			else if (type == CefValueType::VTYPE_BOOL) {
				elem = CefV8Value::CreateBool(val->GetBool(key));
			}
			else if (type == CefValueType::VTYPE_DOUBLE) {
				elem = CefV8Value::CreateDouble(val->GetDouble(key));
			}
			else if (type == CefValueType::VTYPE_NULL) {
				elem = CefV8Value::CreateNull();
			}
			else if (type == CefValueType::VTYPE_STRING) {
				const std::string& s = val->GetString(key);
				to_cef_v8_value_helper(s, elem, functionHandler);
			}
			else {
				throw std::runtime_error("An unsupported type was passed in.");
			}
			out->SetValue(key, elem, CefV8Value::PropertyAttribute::V8_PROPERTY_ATTRIBUTE_NONE);
		});
	}

	void to_cef_v8_value_helper(CefRefPtr<CefListValue> val, CefRefPtr<CefV8Value>& out, const FunctionHandler& functionHandler)
	{
		out = CefV8Value::CreateArray(val->GetSize());
		for (size_t i = 0; i < val->GetSize(); ++i) {
			CefRefPtr<CefV8Value> elem;
			CefValueType type = val->GetType(i);
			if (type == CefValueType::VTYPE_LIST) {
				to_cef_v8_value_helper(val->GetList(i), elem, functionHandler);
			} else if (type == CefValueType::VTYPE_DICTIONARY) {
				to_cef_v8_value_helper(val->GetDictionary(i), elem, functionHandler);
			} else if (type == CefValueType::VTYPE_BOOL) {
				elem = CefV8Value::CreateBool(val->GetBool(i));
			} else if (type == CefValueType::VTYPE_DOUBLE) {
				elem = CefV8Value::CreateDouble(val->GetDouble(i));
			} else if (type == CefValueType::VTYPE_NULL) {
				elem = CefV8Value::CreateNull();
			} else if (type == CefValueType::VTYPE_STRING) {
				const std::string& s = val->GetString(i);
				to_cef_v8_value_helper(s, elem, functionHandler);
			} else {
				throw std::runtime_error("An unsupported type was passed in.");
			}
			out->SetValue(i, elem);
		}
	}

	CefRefPtr<CefListValue> to_ipc_message_arguments_helper(const JSValue::Array& o);
	CefRefPtr<CefDictionaryValue> to_ipc_message_arguments_helper(const JSValue::Object& o);
}


namespace Browser
{
	RenderProcessHandler::RenderProcessHandler()
	{
		_messageHandlers[Util::get_register_api_message_name()] = std::bind(&RenderProcessHandler::on_register_api, this, std::placeholders::_1, std::placeholders::_2);
	}

	bool RenderProcessHandler::OnProcessMessageReceived(CefRefPtr< CefBrowser > browser, CefProcessId source_process, CefRefPtr< CefProcessMessage > message)
	{
		const std::string name = message->GetName();
		auto messageHandlerIter = _messageHandlers.find(name);
		if (messageHandlerIter == std::end(_messageHandlers)) {
			throw std::runtime_error("Could not find an appropriate message handler.");
		}

		messageHandlerIter->second(browser, message->GetArgumentList());
		return true;
	}

	void RenderProcessHandler::on_register_api(CefRefPtr< CefBrowser > browser, CefRefPtr< CefListValue > arguments)
	{
		ContextOpener context(browser->GetMainFrame()->GetV8Context());
		CefRefPtr<CefV8Value> global = context.get()->GetGlobal();
		CefRefPtr<CefV8Value> val;
		to_cef_v8_value_helper(arguments->GetDictionary(0), val, [=](int id){
			CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create("invocation");
			message->GetArgumentList()->SetInt(0, id);
			browser->SendProcessMessage(PID_BROWSER, message);
		});
		global->SetValue(CefString("api"), val, CefV8Value::PropertyAttribute::V8_PROPERTY_ATTRIBUTE_NONE);
	}
}

