// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "simple_app.h"

#include <string>

#include "simple_handler.h"
#include "util.h"
#include "cef/cef_browser.h"
#include "cef/cef_command_line.h"

SimpleApp::SimpleApp(const std::function<void(const CefRenderHandler::RectList&, const void*)>& onPaint) 
:_onPaint(onPaint)
{}

void SimpleApp::OnContextInitialized() {
  REQUIRE_UI_THREAD();

  // Information used when creating the native window.
  CefWindowInfo window_info;

  window_info.SetAsOffScreen(nullptr);

  // SimpleHandler implements browser-level callbacks.
  CefRefPtr<SimpleHandler> handler(new SimpleHandler(1000, 1000, _onPaint));

  // Specify CEF browser settings here.
  CefBrowserSettings browser_settings;

  std::string url;

  // Check if a "--url=" value was provided via the command-line. If so, use
  // that instead of the default URL.
  CefRefPtr<CefCommandLine> command_line =
      CefCommandLine::GetGlobalCommandLine();
  url = command_line->GetSwitchValue("url");
  if (url.empty())
    url = "http://www.jennamichellephotography.com";

  // Create the first browser window.
  CefBrowserHost::CreateBrowser(window_info, handler.get(), url,
                                browser_settings, NULL);
}
