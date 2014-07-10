#pragma once

#include "cef/cef_app.h"
#include "cef/cef_client.h"
#include "cef/cef_render_handler.h"

#include <functional>
#include <list>

class SimpleApp : public CefApp, public CefBrowserProcessHandler, public CefClient, public CefLoadHandler, public CefRenderHandler
{
public:
	SimpleApp(const std::string& indexPath, const std::function<void(const CefRenderHandler::RectList&, const void*)>& onPaint);


	// CefApp methods:
	virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override;

	// CefBrowserProcessHandler methods:
	virtual void OnContextInitialized() override;

	// CefClient methods:
	CefRefPtr<CefLoadHandler> GetLoadHandler() override;
	CefRefPtr<CefRenderHandler> GetRenderHandler() override;

	// CefLoadHandler methods:
	void OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, ErrorCode errorCode, const CefString& errorText, const CefString& failedUrl) override;

	// CefRenderHandler methods:
	bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect) override;
	void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height) override;

	// Request that all existing browser windows close.
	void CloseAllBrowsers(bool forceClose);

private:
	IMPLEMENT_REFCOUNTING(SimpleApp);

	typedef std::list<CefRefPtr<CefBrowser>> BrowserList;
	BrowserList _browserList;

	int _width;
	int _height;
	std::function<void(const RectList&, const void*)> _onPaint;
};
