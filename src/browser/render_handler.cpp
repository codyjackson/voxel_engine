#pragma once

#include "render_handler.h"


RenderHandler::RenderHandler(int width, int height, const std::function<void(const RectList&, const void*)>& onPaint)
	: _width(width), _height(height), _onPaint(onPaint)
{}

bool RenderHandler::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect)
{
	rect = CefRect(0, 0, _width, _height);
	return true;
}
void RenderHandler::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height)
{
	_onPaint(dirtyRects, buffer);
}