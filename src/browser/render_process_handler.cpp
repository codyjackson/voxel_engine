#include "render_process_handler.h"

#include "jsvalue.h"
#include "util.h"

namespace
{
	CefRefPtr<CefListValue> to_cef_list_value(CefRefPtr<CefV8Value> v);
	CefRefPtr<CefDictionaryValue> to_cef_dictionary_value(CefRefPtr<CefV8Value> v)
	{
		CefRefPtr<CefDictionaryValue> out = CefDictionaryValue::Create();
		std::vector<CefString> keys;
		v->GetKeys(keys);
		std::for_each(std::begin(keys), std::end(keys), [&](const CefString& key){
			CefRefPtr<CefV8Value> val = v->GetValue(key);
			if (val->IsArray()) {
				out->SetList(key, to_cef_list_value(val));
			}
			else if (val->IsObject()) {
				out->SetDictionary(key, to_cef_dictionary_value(val));
			}
			else if (val->IsBool()) {
				out->SetBool(key, val->GetBoolValue());
			}
			else if (val->IsDouble()) {
				out->SetDouble(key, val->GetDoubleValue());
			}
			else if (val->IsNull()) {
				out->SetNull(key);
			}
			else if (val->IsString()) {
				out->SetString(key, val->GetStringValue());
			}
			else {
				throw std::runtime_error("Type not supported");
			}
		});

		return out;
	}

	CefRefPtr<CefListValue> to_cef_list_value(CefRefPtr<CefV8Value> v)
	{
		CefRefPtr<CefListValue> out = CefListValue::Create();
		std::vector<CefString> keys;
		v->GetKeys(keys);
		for (int i = 0; i < v->GetArrayLength(); ++i) {
			CefRefPtr<CefV8Value> val = v->GetValue(i);
			if (val->IsArray()) {
				out->SetList(i, to_cef_list_value(val));
			}
			else if (val->IsObject()) {
				out->SetDictionary(i, to_cef_dictionary_value(val));
			}
			else if (val->IsBool()) {
				out->SetBool(i, val->GetBoolValue());
			}
			else if (val->IsDouble()) {
				out->SetDouble(i, val->GetDoubleValue());
			}
			else if (val->IsNull()) {
				out->SetNull(i);
			}
			else if (val->IsString()) {
				out->SetString(i, val->GetStringValue());
			}
			else {
				throw std::runtime_error("Type not supported");
			}
		}

		return out;
	}

	CefRefPtr<CefListValue> to_cef_list_value(const CefV8ValueList& l)
	{
		CefRefPtr<CefListValue> out = CefListValue::Create();
		for (size_t i = 0; i < l.size(); ++i) {
			CefRefPtr<CefV8Value> val = l[i];
			if (val->IsArray()) {
				out->SetList(i, to_cef_list_value(val));
			} else if (val->IsObject()) {
				out->SetDictionary(i, to_cef_dictionary_value(val));
			} else if (val->IsBool()) {
				out->SetBool(i, val->GetBoolValue());
			} else if (val->IsDouble()) {
				out->SetDouble(i, val->GetDoubleValue());
			} else if (val->IsNull()) {
				out->SetNull(i);
			} else if (val->IsString()) {
				out->SetString(i, val->GetStringValue());
			} else {
				throw std::runtime_error("Type not supported");
			}
		}

		return out;
	}


	long generate_deferred_id()
	{
		static long id = 0;
		return id++;
	}

	class MyV8Handler : public CefV8Handler {
	public:
		MyV8Handler(int browserFunctionId, std::unordered_map<int, Browser::Deferred>& idToDeferred, CefRefPtr< CefV8Context > context, CefRefPtr< CefBrowser > browser)
			:_browserFunctionId(browserFunctionId), _idToDeferred(idToDeferred), _context(context), _browser(browser)
		{}

		virtual bool Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception) OVERRIDE
		{
			const auto deferredId = generate_deferred_id();
			CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create("invocation");
			message->GetArgumentList()->SetInt(0, _browserFunctionId);
			message->GetArgumentList()->SetInt(1, deferredId);
			message->GetArgumentList()->SetList(2, to_cef_list_value(arguments));
			_browser->SendProcessMessage(PID_BROWSER, message);
			_idToDeferred.insert(std::make_pair(deferredId, Browser::Deferred(_context)));
			retval = _idToDeferred[deferredId].get_promise();
			return true;
		}

			// Provide the reference counting implementation for this class.
		IMPLEMENT_REFCOUNTING(MyV8Handler);

	private:
		int _browserFunctionId;
		std::unordered_map<int, Browser::Deferred>& _idToDeferred;
		CefRefPtr< CefV8Context > _context;
		CefRefPtr< CefBrowser > _browser;
	};
}


namespace Browser
{
	RenderProcessHandler::RenderProcessHandler()
	{
		_messageHandlers[Util::get_register_api_message_name()] = std::bind(&RenderProcessHandler::on_register_api, this, std::placeholders::_1, std::placeholders::_2);
		_messageHandlers[Util::get_resolve_promise_message_name()] = std::bind(&RenderProcessHandler::on_resolve_promise, this, std::placeholders::_1, std::placeholders::_2);
		_messageHandlers[Util::get_reject_promise_message_name()] = std::bind(&RenderProcessHandler::on_reject_promise, this, std::placeholders::_1, std::placeholders::_2);
	}

	void RenderProcessHandler::OnContextCreated(CefRefPtr< CefBrowser > browser, CefRefPtr< CefFrame > frame, CefRefPtr< CefV8Context > context)
	{
		_context = context;
		_browser = browser;

		if (_api.get() == nullptr) {
			return;
		}
		register_api(context);
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

	void RenderProcessHandler::register_api(CefRefPtr<CefV8Context> context)
	{
		Util::ContextOpener contextOpener(context);
		CefRefPtr<CefV8Value> global = contextOpener.get()->GetGlobal();
		CefRefPtr<CefV8Value> val;
		to_cef_v8_value_helper(_api, val);
		global->SetValue(CefString("api"), val, CefV8Value::PropertyAttribute::V8_PROPERTY_ATTRIBUTE_NONE);
	}

	void RenderProcessHandler::on_register_api(CefRefPtr< CefBrowser > browser, CefRefPtr< CefListValue > arguments)
	{
		_api = arguments->GetDictionary(0)->Copy(false);
		register_api(browser->GetMainFrame()->GetV8Context());
	}

	void RenderProcessHandler::on_resolve_promise(CefRefPtr< CefBrowser > browser, CefRefPtr< CefListValue > arguments)
	{
		CefV8ValueList convertedArgs;
		to_cef_v8_value_list(arguments->GetList(0), convertedArgs);
		const auto iter = _idToDeferred.find(arguments->GetInt(1));
		iter->second.resolve(convertedArgs);
		_idToDeferred.erase(iter);
	}

	void RenderProcessHandler::on_reject_promise(CefRefPtr< CefBrowser > browser, CefRefPtr< CefListValue > arguments)
	{
		CefV8ValueList convertedArgs;
		convertedArgs.push_back(CefV8Value::CreateString(arguments->GetString(0)));
		const auto iter = _idToDeferred.find(arguments->GetInt(1));
		iter->second.reject(convertedArgs);
		_idToDeferred.erase(iter);
	}

	void RenderProcessHandler::to_cef_v8_function(int id, CefRefPtr<CefV8Value>& out)
	{
		out = CefV8Value::CreateFunction("anonymous", new MyV8Handler(id, _idToDeferred, _context, _browser));
	}

	void RenderProcessHandler::to_cef_v8_value_helper(std::string s, CefRefPtr<CefV8Value>& out)
	{
		const std::string& placeholder = Browser::Util::get_ipc_function_string_placeholder();
		auto iter = std::find_first_of(std::begin(s), std::end(s), std::begin(placeholder), std::end(placeholder));
		if (iter != std::end(s)) {
			s.erase(iter, iter + placeholder.size());
			return to_cef_v8_function(std::stoi(s), out);
		}
		out = CefV8Value::CreateString(s);
	}

	void RenderProcessHandler::to_cef_v8_value_helper(CefRefPtr<CefDictionaryValue>& val, CefRefPtr<CefV8Value>& out)
	{
		out = CefV8Value::CreateObject(nullptr);
		CefDictionaryValue::KeyList keys;
		val->GetKeys(keys);
		std::for_each(std::begin(keys), std::end(keys), [&](const std::string& key){
			CefRefPtr<CefV8Value> elem;
			CefValueType type = val->GetType(key);
			if (type == CefValueType::VTYPE_LIST) {
				to_cef_v8_value_helper(val->GetList(key), elem);
			}
			else if (type == CefValueType::VTYPE_DICTIONARY) {
				to_cef_v8_value_helper(val->GetDictionary(key), elem);
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
				to_cef_v8_value_helper(s, elem);
			}
			else {
				throw std::runtime_error("An unsupported type was passed in.");
			}
			out->SetValue(key, elem, CefV8Value::PropertyAttribute::V8_PROPERTY_ATTRIBUTE_NONE);
		});
	}

	void RenderProcessHandler::to_cef_v8_value_helper(CefRefPtr<CefListValue> val, CefRefPtr<CefV8Value>& out)
	{
		out = CefV8Value::CreateArray(val->GetSize());
		for (size_t i = 0; i < val->GetSize(); ++i) {
			CefRefPtr<CefV8Value> elem;
			CefValueType type = val->GetType(i);
			if (type == CefValueType::VTYPE_LIST) {
				to_cef_v8_value_helper(val->GetList(i), elem);
			}
			else if (type == CefValueType::VTYPE_DICTIONARY) {
				to_cef_v8_value_helper(val->GetDictionary(i), elem);
			}
			else if (type == CefValueType::VTYPE_BOOL) {
				elem = CefV8Value::CreateBool(val->GetBool(i));
			}
			else if (type == CefValueType::VTYPE_DOUBLE) {
				elem = CefV8Value::CreateDouble(val->GetDouble(i));
			}
			else if (type == CefValueType::VTYPE_NULL) {
				elem = CefV8Value::CreateNull();
			}
			else if (type == CefValueType::VTYPE_STRING) {
				const std::string& s = val->GetString(i);
				to_cef_v8_value_helper(s, elem);
			}
			else {
				throw std::runtime_error("An unsupported type was passed in.");
			}
			out->SetValue(i, elem);
		}
	}

	void RenderProcessHandler::to_cef_v8_value_list(CefRefPtr<CefListValue> val, CefV8ValueList& out)
	{
		for (size_t i = 0; i < val->GetSize(); ++i) {
			CefRefPtr<CefV8Value> elem;
			CefValueType type = val->GetType(i);
			if (type == CefValueType::VTYPE_LIST) {
				to_cef_v8_value_helper(val->GetList(i), elem);
			}
			else if (type == CefValueType::VTYPE_DICTIONARY) {
				to_cef_v8_value_helper(val->GetDictionary(i), elem);
			}
			else if (type == CefValueType::VTYPE_BOOL) {
				elem = CefV8Value::CreateBool(val->GetBool(i));
			}
			else if (type == CefValueType::VTYPE_DOUBLE) {
				elem = CefV8Value::CreateDouble(val->GetDouble(i));
			}
			else if (type == CefValueType::VTYPE_NULL) {
				elem = CefV8Value::CreateNull();
			}
			else if (type == CefValueType::VTYPE_STRING) {
				const std::string& s = val->GetString(i);
				to_cef_v8_value_helper(s, elem);
			}
			else {
				throw std::runtime_error("An unsupported type was passed in.");
			}
			out.push_back(elem);
		}
	}
}

