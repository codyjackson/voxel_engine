#include "process_handler.h"

#include "application.h"

#include <cef/cef_app.h>

#include <stdlib.h>

namespace
{
	const CefSettings& GetAppSettings()
	{
		static CefSettings as;
		as.single_process = false;
		as.no_sandbox = true;
		as.multi_threaded_message_loop = false;
		as.command_line_args_disabled = true;
		CefString(&as.cache_path) = "ui_cache";
		return as;
	}
}

std::shared_ptr<Browser::ProcessHandler> Browser::ProcessHandler::_instance;
Browser::ProcessHandler::Scoped Browser::ProcessHandler::make()
{
	if (_instance) {
		throw std::runtime_error("You can only make the processhandler one time. It should be made at the start of the application.");
	}

	CefMainArgs args(GetModuleHandle(nullptr));
	CefRefPtr<Application> application = new Application();
	int exitCode = CefExecuteProcess(args, application.get(), nullptr);
	if (exitCode >= 0) {
		exit(exitCode);
	}

	if (!CefInitialize(args, GetAppSettings(), application.get(), nullptr)) {
		throw std::runtime_error("Failed to initialize cef.");
	}

	while (!application->is_initialized()) {
		tick();
	}

	_instance = std::shared_ptr<ProcessHandler>(new ProcessHandler());
	return Scoped();
}

std::shared_ptr<Browser::Browser> Browser::ProcessHandler::create_browser(const Browser::PaintCallbackFunction& paintFn)
{
	if (!_instance) {
		throw std::runtime_error("The process handler must be instantiated before you can create a browser.");
	}

	return std::make_shared<Browser>(paintFn);
}

void Browser::ProcessHandler::tick()
{
	CefDoMessageLoopWork();
}

void Browser::ProcessHandler::destroy()
{
	_instance.reset();
}

Browser::ProcessHandler::~ProcessHandler()
{
	CefShutdown();
}

Browser::ProcessHandler::ProcessHandler()
{}

Browser::ProcessHandler::Scoped::Scoped()
{}

Browser::ProcessHandler::Scoped::~Scoped()
{
	ProcessHandler::destroy();
	CefShutdown();
}
