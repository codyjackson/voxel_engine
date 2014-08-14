#include "application.h"

#include "cef/cef_runnable.h"
#include "util.h"

#include <algorithm>
#include <sstream>
#include <string>

Browser::Application::~Application()
{
	CefShutdown();
}

Browser::Application::Application()
:_isInitialized(false)
{}

bool Browser::Application::is_initialized() const
{
	return _isInitialized;
}

CefRefPtr<CefBrowserProcessHandler> Browser::Application::GetBrowserProcessHandler()
{
	return this;
}

CefRefPtr<CefRenderProcessHandler> Browser::Application::GetRenderProcessHandler()
{
	return this;
}

void Browser::Application::OnContextInitialized() {
  REQUIRE_UI_THREAD();
  _isInitialized = true;
}