#pragma once

#include "browser/browser.h"
#include "input.h"
#include "rendering/texture.h"

#include <functional>
#include <memory>
#include <vector>


typedef unsigned int GLuint;

class UI
{
public:
	UI();

	void update_resolution(const RectSize& resolution);
	const RectSize& get_resolution() const;

	void forward_key_event(Input::Pressable key, Input::PressableState state, int modifiers);
	void forward_mouse_button_event(Input::Pressable button, Input::PressableState state, int modifiers);
	void forward_mouse_move_event(double x, double y);
	void forward_mouse_wheel_event(double xoffset, double yoffset);

	void register_api(const JSValue& api);

	~UI();

	void tick();
	void render();

private:
	void update_texture(const RectSize& fullSize, const CefRenderHandler::RectList& dirtyRects, const void* buffer);

	std::shared_ptr<Texture> _texture;
	std::shared_ptr<Browser::Browser> _browser;
};