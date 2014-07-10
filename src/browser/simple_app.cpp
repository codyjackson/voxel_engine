#include "simple_app.h"

#include "cef/cef_runnable.h"
#include "simple_handler.h"
#include "util.h"

#include <algorithm>
#include <sstream>
#include <string>

SimpleApp::SimpleApp(const std::string& indexPath, const std::function<void(const CefRenderHandler::RectList&, const void*)>& onPaint)
:_onPaint(onPaint)
{}

CefRefPtr<CefBrowserProcessHandler> SimpleApp::GetBrowserProcessHandler()
{
	return this;
}

void SimpleApp::OnContextInitialized() {
  REQUIRE_UI_THREAD();

  CefWindowInfo window_info;

  window_info.SetAsOffScreen(nullptr);
  window_info.SetTransparentPainting(true);

  CefRefPtr<SimpleHandler> handler(new SimpleHandler(1000, 1000, _onPaint));
  CefBrowserSettings browser_settings;

  std::string url = "C:/Users/sxenog/Desktop/test.html";

  // Create the first browser window.
  CefBrowserHost::CreateBrowser(window_info, this, url, browser_settings, NULL);
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
	rect = CefRect(0, 0, 1000, 1000);
	return true;
}

void SimpleApp::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height)
{
	_onPaint(dirtyRects, buffer);
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