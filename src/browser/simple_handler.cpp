// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "simple_handler.h"


#include "util.h"
#include "cef/cef_app.h"
#include "cef/cef_runnable.h"

#include <algorithm>
#include <sstream>
#include <string>

SimpleHandler::SimpleHandler(int width, int height, const std::function<void(const RectList&, const void*)>& onPaint)
:_isClosing(false), _width(width), _height(height), _onPaint(onPaint)
{}

SimpleHandler::~SimpleHandler() 
{}

CefRefPtr<CefLifeSpanHandler> SimpleHandler::GetLifeSpanHandler()
{
	return this;
}
CefRefPtr<CefLoadHandler> SimpleHandler::GetLoadHandler()
{
	return this;
}
CefRefPtr<CefRenderHandler> SimpleHandler::GetRenderHandler()
{
	return this;
}

void SimpleHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser) 
{
	REQUIRE_UI_THREAD();

	// Add to the list of existing browsers.
	_browserList.push_back(browser);
}

bool SimpleHandler::DoClose(CefRefPtr<CefBrowser> browser) 
{
	REQUIRE_UI_THREAD();

	// Closing the main window requires special handling. See the DoClose()
	// documentation in the CEF header for a detailed destription of this
	// process.
	if (_browserList.size() == 1) {
		// Set a flag to indicate that the window close should be allowed.
		_isClosing = true;
	}

	// Allow the close. For windowed browsers this will result in the OS close
	// event being sent.
	return false;
}

void SimpleHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser) 
{
	REQUIRE_UI_THREAD();
	auto iter = std::find_if(std::begin(_browserList), std::end(_browserList), [browser](CefRefPtr<CefBrowser> b){
		return b->IsSame(browser);
	});

	if (iter != std::end(_browserList)) {
		_browserList.erase(iter);
	}
}

void SimpleHandler::OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, ErrorCode errorCode, const CefString& errorText, const CefString& failedUrl) 
{
	REQUIRE_UI_THREAD();

	// Don't display an error for downloaded files.
	if (errorCode == ERR_ABORTED)
		return;

	// Display a load error message.
	std::stringstream ss;
	ss << "<html><body bgcolor=\"white\">"
		"<h2>Failed to load URL " << std::string(failedUrl) <<
		" with error " << std::string(errorText) << " (" << errorCode <<
		").</h2></body></html>";
	frame->LoadString(ss.str(), failedUrl);
}

bool SimpleHandler::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect)
{
	rect = CefRect(0, 0, _width, _height);
	return true;
}

void SimpleHandler::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height)
{
	_onPaint(dirtyRects, buffer);
}

void SimpleHandler::CloseAllBrowsers(bool forceClose) 
{
	if (!CefCurrentlyOn(TID_UI)) {
		CefPostTask(TID_UI, NewCefRunnableMethod(this, &SimpleHandler::CloseAllBrowsers, forceClose));
		return;
	}

	if (_browserList.empty()) {
		return;
	}

	std::for_each(std::begin(_browserList), std::end(_browserList), [forceClose](CefRefPtr<CefBrowser> browser){
		browser->GetHost()->CloseBrowser(forceClose);
	});
}

bool SimpleHandler::IsClosing() const
{ 
	return _isClosing; 
}
