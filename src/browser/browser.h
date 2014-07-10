#pragma once

#include "simple_app.h"

#include "../spatial/rect_size.h"

#include <cef/cef_render_handler.h>
#include <functional>
#include <string>

class Browser
{
public:
	typedef std::function<void(const CefRenderHandler::RectList&, const void*)> PaintCallbackFunction;

	Browser(const std::string& path, const RectSize& viewportSize, const PaintCallbackFunction& onPaint);
	~Browser();

	void tick();

private:
	CefRefPtr<CefApp> _app;
};