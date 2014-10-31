#pragma once

#include "jsvalue.h"

#include "../spatial/rect_size.h"

#include <boost/filesystem.hpp>

#include <cef/cef_app.h>
#include <cef/cef_client.h>

#include <functional>
#include <list>

namespace Browser
{

	class Browser;

	class Application : public CefApp, public CefBrowserProcessHandler, public CefRenderProcessHandler, boost::noncopyable
	{
	public:
		class IJSObjectifiable;
		Application(Browser& browser);
		~Application();

		// CefApp methods:
		virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override;
		virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() override;

		//CefRenderProcessHandler methods:
		virtual void OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) override;

		// CefBrowserProcessHandler methods:
		virtual void OnContextInitialized() override;


	private:
		IMPLEMENT_REFCOUNTING(Application);
		Browser& _browser;
	};

	class Application::IJSObjectifiable
	{
	public:
		virtual JSValue create_js_object() = 0;
	};
}