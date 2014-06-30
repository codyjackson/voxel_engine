#include "browser.h"

#include "simple_app.h"
#include "render_handler.h"

#include <cef/cef_app.h>

#include <exception>
#include <string>

namespace
{
	class BrowserApp : public CefApp
	{
	public:
		IMPLEMENT_REFCOUNTING(BrowserApp);
	};
}

Browser::Browser(const std::string& url, const std::function<void(const CefRenderHandler::RectList&, const void*)>& onPaint)
{
	CefMainArgs args(GetModuleHandle(nullptr));
	CefRefPtr<CefApp> app(new BrowserApp);
	CefSettings appSettings;
	appSettings.no_sandbox = true;

	/*int exitCode = CefExecuteProcess(args, app.get(), nullptr);
	if (exitCode >= 0) {
		throw std::runtime_error("Failed to execute cef process. Error code: " + std::to_string(exitCode));
	}*/

	if (!CefInitialize(args, appSettings, app, nullptr)) {
		throw std::runtime_error("Failed to initialize cef.");
	}

	CefRefPtr<BrowserClient> browserClient(new BrowserClient(new RenderHandler(400, 400, [](const CefRenderHandler::RectList&, const void*){})));
	CefWindowInfo windowInfo;
	windowInfo.SetAsOffScreen(nullptr);
	CefBrowserSettings browserSettings;

	auto test = CefBrowserHost::CreateBrowserSync(windowInfo, browserClient.get(), "http://www.google.com", browserSettings, nullptr);
}

void Browser::tick() 
{
	CefDoMessageLoopWork();
}

Browser::~Browser()
{
	CefShutdown();
}
