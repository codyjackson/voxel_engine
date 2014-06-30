#pragma once

#include <cef/cef_render_handler.h>
#include <functional>

class RenderHandler :  public CefRenderHandler
{
public:
	RenderHandler(int width, int height, const std::function<void(const RectList&, const void*)>& onPaint);

public:
	bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect) override;
	void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height) override;

	// CefBase interface
	IMPLEMENT_REFCOUNTING(RenderHandler);

private:
	int _width;
	int _height;
	std::function<void(const RectList&, const void*)> _onPaint;
};