#pragma once

#include "jsvalue.h"

#include "../spatial/rect_size.h"

#include <boost/filesystem.hpp>

#include <cef/cef_app.h>
#include <cef/cef_client.h>
#include <cef/cef_render_handler.h>

#include <functional>
#include <list>

class Browser : public CefApp, public CefBrowserProcessHandler, public CefClient, public CefLoadHandler, public CefRenderHandler, public CefRenderProcessHandler
{
public:
	class IJSObjectifiable;

	typedef std::function<void(const RectSize& fullSize, const CefRenderHandler::RectList&, const void*)> PaintCallbackFunction;
	static CefRefPtr<Browser> make(const boost::filesystem::path& path, const RectSize& viewportSize, const PaintCallbackFunction& onPaint);
	~Browser();

	void execute_javascript(const std::string& js);

	void update_viewport_size(const RectSize& viewportSize);
	const RectSize& get_viewport_size() const;
	int get_width() const;
	int get_height() const;

	bool is_context_created() const;

	void register_api(const JSValue& api);

	void tick();

private:
	void register_api_impl(JSValue api);

	typedef std::list<CefRefPtr<CefBrowser>> BrowserList;
	BrowserList _browserList;

	std::shared_ptr<JSValue> _api;
	std::string _path;
	std::function<void(const RectSize& fullSize, const RectList&, const void*)> _onPaint;
	RectSize _viewportSize;
	CefRefPtr<CefV8Context> _rootContext;
	CefRefPtr<CefFrame> _frame;

public:
	// CefRenderProcessHandler
	virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() override;
	virtual void OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) override;

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
	IMPLEMENT_REFCOUNTING(Browser);
	Browser(const std::string& indexPath, const RectSize& viewportSize, const std::function<void(const RectSize& fullSize, const RectList&, const void*)>& onPaint);
};

class Browser::IJSObjectifiable
{
public:
	virtual JSValue create_js_object() = 0;
};