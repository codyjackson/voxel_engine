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
	class Application : public CefApp, public CefBrowserProcessHandler, public CefRenderProcessHandler
	{
	public:
		class IJSObjectifiable;
		Application();
		~Application();

		bool is_initialized() const;

		// CefApp methods:
		virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override;
		virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() override;

		// CefBrowserProcessHandler methods:
		virtual void OnContextInitialized() override;


	private:
		IMPLEMENT_REFCOUNTING(Application);

		bool _isInitialized;
	};

	class Application::IJSObjectifiable
	{
	public:
		virtual JSValue create_js_object() = 0;
	};
}