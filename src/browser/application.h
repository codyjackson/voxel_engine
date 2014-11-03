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

	class Application : public CefApp, public CefRenderProcessHandler, boost::noncopyable
	{
	public:
		class IJSObjectifiable;
		Application(const std::function<void(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)>& onContextCreated);
		~Application();

		// CefApp methods:
		virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() override;

	private:
		IMPLEMENT_REFCOUNTING(Application);
		std::function<void(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)> _onContextCreated;
	};
}