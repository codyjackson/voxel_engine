#include "browser.h"

#include "../spatial/rect_size.h"
#include "util.h"

#include <cef/cef_app.h>
#include <cef/cef_client.h>
#include <cef/cef_render_handler.h>
#include <cef/cef_runnable.h>

#include <functional>
#include <exception>
#include <list>
#include <sstream>
#include <string>

Browser::Browser(const boost::filesystem::path& path, const RectSize& viewportSize, const std::function<void(const RectSize& fullSize, const CefRenderHandler::RectList&, const void*)>& onPaint)
:_app(new ApplicationHandlers(path.string(), viewportSize, onPaint))
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

void Browser::execute_javascript(const std::string& js)
{
	_app->execute_javascript(js);
}

void Browser::register_api(const JSValue& api)
{
	_app->register_api(api);
}

void Browser::tick()
{
	CefDoMessageLoopWork();
}

void Browser::update_viewport_size(const RectSize& rectSize)
{
	_app.get()->update_viewport_size(rectSize);
}