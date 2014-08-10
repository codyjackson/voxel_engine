#pragma once

#include "../spatial/rect_size.h"

#include "application_handlers.h"
#include "jsvalue.h"

#include <boost/filesystem.hpp>
#include <cef/cef_render_handler.h>
#include <functional>
#include <string>
#include <type_traits>

class Browser
{
public:
	typedef std::function<void(const RectSize& fullSize, const CefRenderHandler::RectList&, const void*)> PaintCallbackFunction;
	typedef std::function<CefRefPtr<CefV8Value>(CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments)> Function;

	class IJSObjectifiable;

	Browser(const boost::filesystem::path& path, const RectSize& viewportSize, const PaintCallbackFunction& onPaint);
	~Browser();

	void update_viewport_size(const RectSize& rectSize);

	void execute_javascript(const std::string& javascript);
	void register_api(const JSValue& api);

	int get_width() const;
	int get_height() const;

	void tick();

private:
	void register_api_impl(JSValue api);

	CefRefPtr<ApplicationHandlers> _app;
};

class Browser::IJSObjectifiable
{
public:
	virtual JSValue create_js_object() = 0;
};