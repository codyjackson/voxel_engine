#include "application.h"

#include "browser.h"
#include "cef/cef_runnable.h"
#include "jsvalue.h"
#include "util.h"

#include <algorithm>
#include <sstream>
#include <string>

Browser::Application::~Application()
{
	CefShutdown();
}

Browser::Application::Application(Browser& browser)
:_browser(browser)
{}

CefRefPtr<CefBrowserProcessHandler> Browser::Application::GetBrowserProcessHandler()
{
	return this;
}

CefRefPtr<CefRenderProcessHandler> Browser::Application::GetRenderProcessHandler()
{
	return this;
}

void Browser::Application::OnContextCreated(CefRefPtr<CefBrowser> rawBrowser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
{
	CefRefPtr<CefV8Value> object = context->GetGlobal();
	object->SetValue("api", JSValue::to_cef_v8_value(_browser.get_api()), V8_PROPERTY_ATTRIBUTE_NONE);
}

void Browser::Application::OnContextInitialized() {
  REQUIRE_UI_THREAD();
}