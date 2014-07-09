#pragma once

#include "cef/cef_client.h"

#include <functional>
#include <list>

class SimpleHandler : public CefClient, public CefLifeSpanHandler, public CefLoadHandler, public CefRenderHandler
{
public:
	SimpleHandler(int width, int height, const std::function<void(const RectList&, const void*)>& onPaint);
	~SimpleHandler();

	// CefClient methods:
	virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override;
	virtual CefRefPtr<CefLoadHandler> GetLoadHandler() override;
	virtual CefRefPtr<CefRenderHandler> GetRenderHandler() override;

	// CefLifeSpanHandler methods:
	virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;
	virtual bool DoClose(CefRefPtr<CefBrowser> browser) override;
	virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;

	// CefLoadHandler methods:
	virtual void OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, ErrorCode errorCode, const CefString& errorText, const CefString& failedUrl) override;

	// CefRenderHandler methods:
	bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect) override;
	void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height) override;

	// Request that all existing browser windows close.
	void CloseAllBrowsers(bool forceClose);

	bool IsClosing() const;

private:
	// List of existing browser windows. Only accessed on the CEF UI thread.
	typedef std::list<CefRefPtr<CefBrowser> > BrowserList;
	BrowserList _browserList;

	bool _isClosing;

	int _width;
	int _height;
	std::function<void(const RectList&, const void*)> _onPaint;

	// Include the default reference counting implementation.
	IMPLEMENT_REFCOUNTING(SimpleHandler);
};
