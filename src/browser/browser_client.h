#pragma once

#include "render_handler.h"

#include <cef/cef_client.h>

class BrowserClient : public CefClient
{
public:
	BrowserClient(CefRefPtr<CefRenderHandler> renderHandler);

	virtual CefRefPtr<CefRenderHandler> GetRenderHandler() override;

	IMPLEMENT_REFCOUNTING(BrowserClient);

private:
	CefRefPtr<CefRenderHandler> _renderHandler;
};