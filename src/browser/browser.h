#pragma once

#include "jsvalue.h"

#include "../spatial/rect_size.h"
#include "../input.h"

#include "application.h"

#include <boost/noncopyable.hpp>
#include <cef/cef_client.h>
#include <cef/cef_render_process_handler.h>

namespace Browser
{
	class Browser : boost::noncopyable
	{
	public:
		typedef std::function<void(const RectSize& fullSize, const CefRenderHandler::RectList& dirtyRects, const void* dataBuffer)> PaintCallbackFunction;
		static std::shared_ptr<Browser> make(const PaintCallbackFunction& onPaint);
		~Browser();

		void load_url(const std::string& url);
		void update_viewport_size(const RectSize& viewportSize);

		void register_api(const JSValue& api);

		void forward_key_event(Input::Pressable key, Input::PressableState state, int modifiers);
		void forward_mouse_button_event(Input::Pressable button, Input::PressableState state, int modifiers);
		void forward_mouse_move_event(double x, double y);
		void forward_mouse_wheel_event(double xoffset, double yoffset);

		void tick();

		const JSValue& get_api() const;

		CefRefPtr<CefListValue> to_ipc_message_arguments_helper(const JSValue::Array& o);
		CefRefPtr<CefDictionaryValue> to_ipc_message_arguments_helper(const JSValue::Object& o);
	private:
		void on_context_created(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context);
		

		static std::shared_ptr<Browser> _instance;
		Browser(const PaintCallbackFunction& onPaint);
		class Handler;
		CefRefPtr<Application> _application;
		CefRefPtr<Handler> _handler;
		CefRefPtr<CefBrowser> _browser;
		PaintCallbackFunction _onPaint;
		CefRect _viewportRect;
		CefMouseEvent _mouseEvent;
		CefRefPtr<CefV8Context> _context;
		std::unordered_map<int, JSValue::Function> _idToIPCFunction;
	};

	class Browser::Handler : public CefClient, public CefRenderHandler, boost::noncopyable
	{
	public:
		Handler(Browser& browser);

		//CefClient methods:
		CefRefPtr<CefRenderHandler> GetRenderHandler() override;
		//CefRefPtr< CefLifeSpanHandler > GetLifeSpanHandler() override;
		bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message) override;

		//CefRenderHandler methods:
		bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect) override;
		void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height) override;

	private:
		Browser& _browser;
		IMPLEMENT_REFCOUNTING(Handler);
	};
}