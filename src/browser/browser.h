#pragma once

#include "simple_app.h"

#include <cef/cef_render_handler.h>
#include <functional>
#include <string>

class Browser
{
public:
	Browser(const std::string& path, const std::function<void(const CefRenderHandler::RectList&, const void*)>& onPaint);
	~Browser();

	void tick();

private:
	CefRefPtr<CefApp> _app;
};