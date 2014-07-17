#include "simple_app.h"

#include "cef/cef_runnable.h"
#include "util.h"

#include <algorithm>
#include <sstream>
#include <string>

SimpleApp::SimpleApp(const std::string& indexPath, const RectSize& viewportSize, const std::function<void(const RectSize& fullSize, const CefRenderHandler::RectList&, const void*)>& onPaint)
:_path(indexPath), _onPaint(onPaint), _viewportSize(viewportSize)
{}

void SimpleApp::update_viewport_size(const RectSize& viewportSize)
{
	_viewportSize = viewportSize;
}

CefRefPtr<CefBrowserProcessHandler> SimpleApp::GetBrowserProcessHandler()
{
	return this;
}

void SimpleApp::OnContextInitialized() {
  REQUIRE_UI_THREAD();

  CefWindowInfo windowInfo;

  windowInfo.SetAsOffScreen(nullptr);
  windowInfo.SetTransparentPainting(true);

  CefBrowserSettings browserSettings;

  // Create the first browser window.
  CefBrowserHost::CreateBrowser(windowInfo, this, _path, browserSettings, NULL);
}

CefRefPtr<CefLoadHandler> SimpleApp::GetLoadHandler()
{
	return this;
}

CefRefPtr<CefRenderHandler> SimpleApp::GetRenderHandler()
{
	return this;
}

void SimpleApp::OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, ErrorCode errorCode, const CefString& errorText, const CefString& failedUrl)
{
	REQUIRE_UI_THREAD();

	// Don't display an error for downloaded files.
	if (errorCode == ERR_ABORTED) {
		return;
	}

	// Display a load error message.
	std::stringstream ss;
	ss << "<html><body bgcolor=\"white\">"
		<< "<h2>Failed to load UI</h2> "
		<< "<h3>Error: "
		<< std::string(errorText)
		<< " (" << errorCode
		<< ")</body></html>";
	frame->LoadString(ss.str(), failedUrl);
}

bool SimpleApp::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect)
{
	rect = CefRect(0, 0, _viewportSize.width, _viewportSize.height);
	return true;
}

void SimpleApp::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height)
{
	_onPaint(RectSize(width, height), dirtyRects, buffer);
}

void SimpleApp::CloseAllBrowsers(bool forceClose)
{
	if (!CefCurrentlyOn(TID_UI)) {
		CefPostTask(TID_UI, NewCefRunnableMethod(this, &SimpleApp::CloseAllBrowsers, forceClose));
		return;
	}

	if (_browserList.empty()) {
		return;
	}

	std::for_each(std::begin(_browserList), std::end(_browserList), [forceClose](CefRefPtr<CefBrowser> browser){
		browser->GetHost()->CloseBrowser(forceClose);
	});
}