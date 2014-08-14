#include "browser.h"

namespace
{

	const CefWindowInfo& GetWindowInfo()
	{
		static CefWindowInfo wi;
		wi.SetAsOffScreen(nullptr);
		wi.SetTransparentPainting(true);
		return wi;
	}

	const CefBrowserSettings& GetBrowserSettings()
	{
		static CefBrowserSettings bs;
		return bs;
	}
}
	

Browser::Browser::Browser(const PaintCallbackFunction& onPaint)
:_handler(new Handler(*this)), _browser(CefBrowserHost::CreateBrowserSync(GetWindowInfo(), _handler.get(), "", GetBrowserSettings(), nullptr)), _onPaint(onPaint)
{}

Browser::Browser::~Browser()
{
	_browser->GetHost()->CloseBrowser(true);
}

void Browser::Browser::load_url(const std::string& url)
{
	_browser->GetMainFrame()->LoadURL(url);
}

void Browser::Browser::register_api(const JSValue& api)
{

}

void Browser::Browser::update_viewport_size(const RectSize& viewportSize)
{
	_viewportRect = CefRect(0, 0, viewportSize.width, viewportSize.height);
}

void Browser::Browser::forward_key_event(Input::Pressable key, Input::PressableState state, int modifiers)
{
	CefKeyEvent ev;
	ev.character = 0;
	ev.unmodified_character = 0;
	ev.native_key_code = static_cast<int>(key);
	ev.modifiers = modifiers;
	ev.type = state == Input::PressableState::UP ? KEYEVENT_KEYUP : KEYEVENT_KEYDOWN;
	_browser->GetHost()->SendKeyEvent(ev);
}

void Browser::Browser::forward_mouse_button_event(Input::Pressable button, Input::PressableState state, int modifiers)
{
	CefBrowserHost::MouseButtonType b;
	if (button == Input::Pressable::MOUSE_BUTTON_1) {
		b = CefBrowserHost::MouseButtonType::MBT_LEFT;
	} else if (button == Input::Pressable::MOUSE_BUTTON_2) {
		b = CefBrowserHost::MouseButtonType::MBT_RIGHT;
	} else if (button == Input::Pressable::MOUSE_BUTTON_3) {
		b = CefBrowserHost::MouseButtonType::MBT_MIDDLE;
	}
	const bool isButtonUp = state == Input::PressableState::UP;
	_mouseEvent.modifiers = modifiers;
	_browser->GetHost()->SendMouseClickEvent(_mouseEvent, b, isButtonUp, 1);
}

void Browser::Browser::forward_mouse_move_event(double x, double y)
{
	_mouseEvent.x = static_cast<int>(x);
	_mouseEvent.y = static_cast<int>(y);
	_browser->GetHost()->SendMouseMoveEvent(_mouseEvent, false);
}

void Browser::Browser::forward_mouse_wheel_event(double xoffset, double yoffset)
{
	_browser->GetHost()->SendMouseWheelEvent(_mouseEvent, static_cast<int>(xoffset), static_cast<int>(yoffset));
}

Browser::Browser::Handler::Handler(Browser& browser)
:_browser(browser)
{}

CefRefPtr<CefRenderHandler> Browser::Browser::Handler::GetRenderHandler()
{
	return this;
}

void Browser::Browser::Handler::OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
{

}

bool Browser::Browser::Handler::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect)
{
	rect = _browser._viewportRect;
	return true;
}

void Browser::Browser::Handler::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height)
{
	_browser._onPaint(RectSize(width, height), dirtyRects, buffer);
}