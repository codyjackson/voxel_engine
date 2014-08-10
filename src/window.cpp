#include "window.h"

Window::Window()
{
	if (!glfwInit()) {
		throw std::runtime_error("Failed to initialize glfw.");
	}
	if (!(_window = glfwCreateWindow(1024, 768, "", NULL, NULL))) {
		throw std::runtime_error("Failed to create window.");
	}
	glfwHideWindow(_window);

	//Remember to clear callbacks in the destructor
	glfwSetKeyCallback(_window, Window::on_keyboard_message_forwarder);
	glfwSetCursorPosCallback(_window, Window::on_mouse_position_message_forwarder);
	glfwSetMouseButtonCallback(_window, Window::on_mouse_button_message_forwarder);
	glfwSetScrollCallback(_window, Window::on_mouse_scroll_wheel_message_forwarder);
	_glfwWindowToWindowMappingForStaticCallbacks.emplace(_window, this);
}

Window::~Window()
{
	if (!_window) {
		return;
	}

	glfwSetKeyCallback(_window, nullptr);
	glfwSetCursorPosCallback(_window, nullptr);
	glfwSetMouseButtonCallback(_window, nullptr);
	glfwSetScrollCallback(_window, nullptr);
	_glfwWindowToWindowMappingForStaticCallbacks.erase(_window);

	glfwDestroyWindow(_window);
	glfwTerminate();
}

void Window::open()
{
	glfwShowWindow(_window);
	glfwMakeContextCurrent(_window);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void Window::render()
{
	_ui.render();
	glfwSwapBuffers(_window);
}


void Window::close()
{
	glfwSetWindowShouldClose(_window, GL_TRUE);
	glfwHideWindow(_window);
}

void Window::update_title(const std::string& title)
{
	glfwSetWindowTitle(_window, title.c_str());
}

void Window::update_resolution(const RectSize& resolution)
{
	glfwSetWindowSize(_window, resolution.width, resolution.height);
	_ui.update_resolution(resolution);
}

RectSize Window::get_resolution() const
{
	RectSize resolution(0, 0);
	glfwGetWindowSize(_window, &resolution.width, &resolution.height);
	return resolution;
}

const int Window::get_width() const
{
	int width;
	int trash;
	glfwGetWindowSize(_window, &width, &trash);
	return width;
}

const int Window::get_height() const
{
	int height;
	int trash;
	glfwGetWindowSize(_window, &trash, &height);
	return height;
}

const glm::ivec2 Window::get_center() const
{
	return glm::ivec2(get_width() / 2, get_height()/2);
}

Input& Window::input()
{
	return _input;
}

UI& Window::ui()
{
	return _ui;
}

const bool Window::is_open() const 
{
	return !glfwWindowShouldClose(_window);
}

void Window::tick()
{
	glfwSetInputMode(_window, GLFW_CURSOR, _input._mouse.is_mouse_hidden() ? GLFW_CURSOR_HIDDEN : GLFW_CURSOR_NORMAL);
	_input.prepare_for_updates();
	glfwPollEvents();
	_ui.tick();
}

void Window::on_keyboard_message_forwarder(GLFWwindow* glfwWindow, int key, int scancode, int action, int modifiers)
{
	if (action == GLFW_REPEAT) {
		return;
	}

	Window& window = *Window::_glfwWindowToWindowMappingForStaticCallbacks[glfwWindow];
	const Input::PressableState state = static_cast<Input::PressableState>(action);
	const Input::Pressable terminal = static_cast<Input::Pressable>(key);
	window._input.update(terminal, state);
}

void Window::on_mouse_position_message_forwarder(GLFWwindow* glfwWindow, double x, double y)
{
	Window& window = *Window::_glfwWindowToWindowMappingForStaticCallbacks[glfwWindow];
	if (!window._input._mouse.is_movement_locked()) {
		return window._input.update_mouse_position(glm::ivec2(static_cast<int>(x), static_cast<int>(y)));
	}

	const glm::ivec2 center = window.get_center();
	window._input.update_mouse_locked_position(center, glm::ivec2(static_cast<int>(x), static_cast<int>(y)));
	glfwSetCursorPos(glfwWindow, center.x, center.y);
}

void Window::on_mouse_button_message_forwarder(GLFWwindow* glfwWindow, int button, int action, int modifiers)
{
	Window& window = *Window::_glfwWindowToWindowMappingForStaticCallbacks[glfwWindow];
	const Input::PressableState state = static_cast<Input::PressableState>(action);
	const Input::Pressable terminal = static_cast<Input::Pressable>(button + static_cast<int>(Input::Pressable::MOUSE_BUTTON_1));
	window._input.update(terminal, state);
}

void Window::on_mouse_scroll_wheel_message_forwarder(GLFWwindow* glfwWindow, double xoffset, double yoffset)
{
	Window& window = *Window::_glfwWindowToWindowMappingForStaticCallbacks[glfwWindow];
	window._input.update_mouse_scroll_wheel(static_cast<int>(yoffset));
}

std::unordered_map<GLFWwindow*, Window*> Window::_glfwWindowToWindowMappingForStaticCallbacks = std::unordered_map<GLFWwindow*, Window*>();