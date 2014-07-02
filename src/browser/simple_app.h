// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef CEF_TESTS_CEFSIMPLE_SIMPLE_APP_H_
#define CEF_TESTS_CEFSIMPLE_SIMPLE_APP_H_

#include "cef/cef_app.h"
#include "cef/cef_render_handler.h"
#include <functional>

class SimpleApp : public CefApp,
                  public CefBrowserProcessHandler {
 public:
	 SimpleApp(const std::function<void(const CefRenderHandler::RectList&, const void*)>& onPaint);
	 

  // CefApp methods:
  virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler()
      OVERRIDE { return this; }

  // CefBrowserProcessHandler methods:
  virtual void OnContextInitialized() OVERRIDE;

 private:
  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(SimpleApp);
  std::function<void(const CefRenderHandler::RectList&, const void*)> _onPaint;
};

#endif  // CEF_TESTS_CEFSIMPLE_SIMPLE_APP_H_
