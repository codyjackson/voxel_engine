#pragma once

#include <cef/cef_app.h>

#include <functional>
#include <unordered_map>

namespace Browser
{
	class RenderProcessHandler : public CefRenderProcessHandler
	{
	public:
		RenderProcessHandler();

		virtual bool OnProcessMessageReceived(CefRefPtr< CefBrowser > browser, CefProcessId source_process, CefRefPtr< CefProcessMessage > message) override;

	private:
		void on_register_api(CefRefPtr< CefBrowser > browser, CefRefPtr< CefListValue > arguments);

		std::unordered_map<std::string, std::function<void(CefRefPtr< CefBrowser > browser, CefRefPtr<CefListValue>)>> _messageHandlers;
		IMPLEMENT_REFCOUNTING(RenderProcessHandler);
	};
}