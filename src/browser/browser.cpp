#include "browser.h"



#include <cef/cef_app.h>

#include <exception>
#include <string>

Browser::Browser(const boost::filesystem::path& path, const RectSize& viewportSize, const std::function<void(const RectSize& fullSize, const CefRenderHandler::RectList&, const void*)>& onPaint)
:_app(new SimpleApp(path.string(), viewportSize, onPaint))
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

	if (!CefInitialize(args, appSettings, _app.get(), nullptr)) {
		throw std::runtime_error("Failed to initialize cef.");
	}
}

Browser::~Browser()
{
	CefShutdown();
}

void Browser::tick()
{
	CefDoMessageLoopWork();
}

void Browser::update_viewport_size(const RectSize& rectSize)
{
	_app.get()->update_viewport_size(rectSize);
}
