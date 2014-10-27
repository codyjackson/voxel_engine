#include "ui.h"

#include "browser/process_handler.h"
#include "constants/runtime_file_paths.h"

#include <Windows.h>
#include <GLFW/glfw3.h>

#include <algorithm>
#include <boost/filesystem.hpp>

UI::UI()
:_browser(Browser::ProcessHandler::create_browser(JSValue(), std::bind(&UI::update_texture, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)))
{
	_browser->load_url(Constants::RuntimeFilePaths::ui_index.string());
	//_browser->load_url("www.google.com");
}

UI::~UI()
{}

void UI::update_resolution(const RectSize& resolution)
{
	_browser->update_viewport_size(resolution);
}

void UI::tick() 
{ 
	Browser::ProcessHandler::tick();
}  

void UI::forward_key_event(Input::Pressable key, Input::PressableState state, int modifiers)
{
	_browser->forward_key_event(key, state, modifiers);
}

void UI::forward_mouse_button_event(Input::Pressable button, Input::PressableState state, int modifiers)
{
	_browser->forward_mouse_button_event(button, state, modifiers);
}

void UI::forward_mouse_move_event(double x, double y)
{
	_browser->forward_mouse_move_event(x, y);
}

void UI::forward_mouse_wheel_event(double xoffset, double yoffset)
{
	_browser->forward_mouse_wheel_event(xoffset, yoffset);
}

void UI::render()
{
	if (!_texture) {
		return;
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	{
		Texture::Binder binder(_texture->bind());
		glLoadIdentity();
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, -1.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0, -1.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, 1.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, 1.0f);
		glEnd();
	}

	glDisable(GL_BLEND);
}

void UI::update_texture(const RectSize& fullSize, const CefRenderHandler::RectList& dirtyRects, const void* buffer)
{
	if (!_texture) { 
		_texture = std::make_shared<Texture>(fullSize, PixelFormat::BGRA, PixelDataType::UNSIGNED_BYTE, buffer);
	}

	if (fullSize != _texture->get_size()) {
		return _texture->resize(fullSize, buffer);
	}

	std::for_each(std::begin(dirtyRects), std::end(dirtyRects), [this, buffer, &fullSize](const CefRect& dirtyRect) {
		_texture->update_pixel_rect(Offset(dirtyRect.x, dirtyRect.y), RectSize(dirtyRect.width, dirtyRect.height), buffer);
	});
} 