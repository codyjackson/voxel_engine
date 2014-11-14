#include "application.h"

#include "browser.h"
#include "cef/cef_runnable.h"
#include "jsvalue.h"
#include "render_process_handler.h"
#include "util.h"

#include <algorithm>
#include <sstream>
#include <string>

Browser::Application::~Application()
{
	CefShutdown();
}

Browser::Application::Application(const std::function<void(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)>& onContextCreated)
:_renderProcessHandler(new RenderProcessHandler())
{}


CefRefPtr<CefRenderProcessHandler> Browser::Application::GetRenderProcessHandler()
{
	return _renderProcessHandler;
}