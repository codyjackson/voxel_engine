#include "application.h"

#include "browser.h"
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

void Browser::Application::OnContextCreated(CefRefPtr<CefBrowser> rawBrowser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
{
	//// Retrieve the context's window object.
	//CefRefPtr<CefV8Value> object = context->GetGlobal();

	//// Create an instance of my CefV8Handler object.
	//CefRefPtr<CefV8Handler> handler = new MyV8Handler();

	//// Create the "myfunc" function.
	//CefRefPtr<CefV8Value> func = CefV8Value::CreateFunction("myfunc", handler);

	//// Add the "myfunc" function to the "window" object.
	//object->SetValue("api", func, V8_PROPERTY_ATTRIBUTE_NONE);
}

void Browser::Application::OnContextInitialized() {
  REQUIRE_UI_THREAD();
  _isInitialized = true;
}