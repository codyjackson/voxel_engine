#pragma once

#include "deferred.h"

#include <cef/cef_app.h>

#include <functional>
#include <map>
#include <memory>
#include <unordered_map>

namespace Browser
{
	class RenderProcessHandler : public CefRenderProcessHandler
	{
	public:
		RenderProcessHandler();

		virtual void OnContextCreated(CefRefPtr< CefBrowser > browser, CefRefPtr< CefFrame > frame, CefRefPtr< CefV8Context > context) override;
		virtual bool OnProcessMessageReceived(CefRefPtr< CefBrowser > browser, CefProcessId source_process, CefRefPtr< CefProcessMessage > message) override;

	private:
		void register_api(CefRefPtr<CefV8Context> context);

		void on_register_api(CefRefPtr< CefBrowser > browser, CefRefPtr< CefListValue > arguments);
		void on_resolve_promise(CefRefPtr< CefBrowser > browser, CefRefPtr< CefListValue > arguments);
		void on_reject_promise(CefRefPtr< CefBrowser > browser, CefRefPtr< CefListValue > arguments);

		void to_cef_v8_function(int id, CefRefPtr<CefV8Value>& out);
		void to_cef_v8_string_helper(std::string s, CefRefPtr<CefV8Value>& out);
		void to_cef_v8_dictionary_helper(CefRefPtr<CefDictionaryValue>& val, CefRefPtr<CefV8Value>& out);
		void to_cef_v8_list_helper(CefRefPtr<CefListValue> val, CefRefPtr<CefV8Value>& out);
		void to_cef_v8_value_list(CefRefPtr<CefListValue> val, CefV8ValueList& out);

		CefRefPtr<CefDictionaryValue> _api;
		std::unordered_map<int, Deferred> _idToDeferred;
		CefRefPtr< CefV8Context > _context;
		CefRefPtr< CefBrowser > _browser;
		std::unordered_map<std::string, std::function<void(CefRefPtr< CefBrowser > browser, CefRefPtr<CefListValue>)>> _messageHandlers;
		IMPLEMENT_REFCOUNTING(RenderProcessHandler);
	};
}