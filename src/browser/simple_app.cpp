// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "simple_app.h"

#include <string>

#include "simple_handler.h"
#include "util.h"
#include "cef/cef_browser.h"
#include "cef/cef_command_line.h"

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
  CefBrowserHost::CreateBrowser(window_info, handler.get(), url, browser_settings, NULL);
}
