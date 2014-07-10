#include "browser.h"



#include <cef/cef_app.h>

#include <exception>
#include <string>

Browser::Browser(const std::string& path, const RectSize& viewportSize, const std::function<void(const CefRenderHandler::RectList&, const void*)>& onPaint)
:_app(new SimpleApp(path, viewportSize, onPaint))
{
	CefMainArgs args(GetModuleHandle(nullptr));
	CefSettings appSettings;
	appSettings.single_process = true;
	appSettings.no_sandbox = true;
	CefString(&appSettings.cache_path) = "ui_cache";

	int exitCode = CefExecuteProcess(args, _app.get(), nullptr);
	if (exitCode >= 0) {
		throw std::runtime_error("Failed to execute cef process. Error code: " + std::to_string(exitCode));
	}

	if (!CefInitialize(args, appSettings, _app, nullptr)) {
		throw std::runtime_error("Failed to initialize cef.");
	}
}

void Browser::tick() 
{
	CefDoMessageLoopWork();
}

Browser::~Browser()
{
	CefShutdown();
}
