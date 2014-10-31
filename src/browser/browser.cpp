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

	const CefSettings& GetAppSettings()
	{
		static CefSettings as;
		as.single_process = true;
		as.no_sandbox = true;
		as.multi_threaded_message_loop = false;
		as.command_line_args_disabled = true;
		CefString(&as.cache_path) = "ui_cache";
		return as;
	}
}
	

std::shared_ptr<Browser::Browser> Browser::Browser::_instance(nullptr);

Browser::Browser::Browser(const JSValue& api, const PaintCallbackFunction& onPaint)
:_handler(new Handler(*this)), _onPaint(onPaint)
{
	CefMainArgs args(GetModuleHandle(nullptr));
	_application = new Application(*this);
	int exitCode = CefExecuteProcess(args, _application.get(), nullptr);
	if (exitCode >= 0) {
		exit(exitCode);
	}

	if (!CefInitialize(args, GetAppSettings(), _application.get(), nullptr)) {
		throw std::runtime_error("Failed to initialize cef.");
	}

	_browser = CefBrowserHost::CreateBrowserSync(GetWindowInfo(), _handler.get(), "", GetBrowserSettings(), nullptr);
}

std::shared_ptr<Browser::Browser> Browser::Browser::make(const JSValue& api, const PaintCallbackFunction& onPaint)
{
	if (_instance) {
		throw std::runtime_error("Only one browser instance can be instantiated at a time.");
	}
	return _instance = std::shared_ptr<Browser>(new Browser(api, onPaint));
}

Browser::Browser::~Browser()
{
	_browser->GetHost()->CloseBrowser(true);
	CefShutdown();
}

void Browser::Browser::load_url(const std::string& url)
{
	_browser->GetMainFrame()->LoadURL(url);
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

const JSValue& Browser::Browser::get_api() const
{
	return _api;
}

void Browser::Browser::tick()
{
	CefDoMessageLoopWork();
}

Browser::Browser::Handler::Handler(Browser& browser)
:_browser(browser)
{}

CefRefPtr<CefRenderHandler> Browser::Browser::Handler::GetRenderHandler()
{
	return this;
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