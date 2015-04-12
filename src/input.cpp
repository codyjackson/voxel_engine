#include "input.h"
#include "window.h"

#include <algorithm>
#include <numeric>

Input::Mouse::Mouse(Window& window)
:_window(window)
{}

void Input::Mouse::hide_cursor()
{
	glfwSetInputMode(_window._window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

void Input::Mouse::show_cursor()
{
	glfwSetInputMode(_window._window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

bool Input::Mouse::is_cursor_hidden() const
{
	return glfwGetInputMode(_window._window, GLFW_CURSOR) == GLFW_CURSOR_HIDDEN;
}

void Input::Mouse::lock_movement(int x, int y)
{
	_lockedPosition = glm::ivec2(x, y);
	update_mouse_movement();
}

void Input::Mouse::unlock_movement()
{
	_lockedPosition.reset();
}

bool Input::Mouse::is_movement_locked() const
{
	return static_cast<bool>(_lockedPosition);
}

JSValue Input::Mouse::create_ui_api()
{
	JSValue::Object o;
	o["lockMovement"] = [this](const JSValue::Array& args){
		if ((args.size() != 2) || !args[0].is_double() || !args[1].is_double()) {
			throw std::runtime_error("Expected two arguments representing the x and y coordinates of where we will lock the cursor position.");
		}

		const int x = static_cast<int>(static_cast<double>(args[0]));
		const int y = static_cast<int>(static_cast<double>(args[1]));
		lock_movement(x, y);
		return JSValue::Array();
	};
		
	o["unlockMovement"] = JSValue::wrap_void_function(std::bind(&Input::Mouse::unlock_movement, this));
	o["hideCursor"] = JSValue::wrap_void_function(std::bind(&Input::Mouse::hide_cursor, this));
	o["showCursor"] = JSValue::wrap_void_function(std::bind(&Input::Mouse::show_cursor, this));
	return o;
}


void Input::Mouse::update_mouse_movement()
{
	if (_lockedPosition) {
		glfwSetCursorPos(_window._window, static_cast<double>(_lockedPosition->x), static_cast<double>(_lockedPosition->y));
	}
}

JSValue Input::create_ui_api()
{
	JSValue::Object o;
	o["mouse"] = _mouse.create_ui_api();
	return o;
}

Input::Mouse& Input::mouse()
{
	return _mouse;
}


Input::Input(Window& window)
:_mouse(window)
{}

void Input::update_mouse_movement()
{
	_mouse.update_mouse_movement();
}
