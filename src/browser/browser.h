#pragma once

#include "simple_app.h"

#include "../spatial/rect_size.h"

#include <boost/filesystem.hpp>
#include <cef/cef_render_handler.h>
#include <functional>
#include <string>

class Browser
{
public:
	typedef std::function<void(const RectSize& fullSize, const CefRenderHandler::RectList&, const void*)> PaintCallbackFunction;

	Browser(const boost::filesystem::path& path, const RectSize& viewportSize, const PaintCallbackFunction& onPaint);
	~Browser();

	void update_viewport_size(const RectSize& rectSize);

	void execute_javascript(const std::string& javascript);

	int get_width() const;
	int get_height() const;

	void tick();

private:
	CefRefPtr<SimpleApp> _app;
};