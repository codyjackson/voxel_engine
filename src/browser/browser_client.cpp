#include "browser_client.h"


BrowserClient::BrowserClient(CefRefPtr<CefRenderHandler> renderHandler)
	: _renderHandler(renderHandler)
{}

CefRefPtr<CefRenderHandler> BrowserClient::GetRenderHandler() 
{
	return _renderHandler;
}