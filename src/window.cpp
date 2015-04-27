#include "window.h"

Window::Window()
:_input(*this)
{
	if (!glfwInit()) {
		throw std::runtime_error("Failed to initialize glfw.");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

	if (!(_window = glfwCreateWindow(1024, 768, "", NULL, NULL))) {
		throw std::runtime_error("Failed to create window.");
	}
	glfwMakeContextCurrent(_window);

	glewExperimental = GL_TRUE;
	auto returnCode = glewInit();
	if ( returnCode != GLEW_OK) {
		auto error = glewGetErrorString(returnCode);
		throw std::runtime_error("Failed to initialize GLEW.");
	}

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
}

void Window::on_key_event(const std::function<void(Input::Pressable key, Input::PressableState state, int modifiers)>& fn)
{
	_onKeyEvent = fn;
}

void Window::on_mouse_button_event(const std::function<void(Input::Pressable button, Input::PressableState state, int modifiers)>& fn)
{
	_onMouseButtonEvent = fn;
}

void Window::on_mouse_move_event(const std::function<void(double x, double y)>& fn)
{
	_onMouseMoveEvent = fn;
}

void Window::on_mouse_wheel_event(const std::function<void(double xoffset, double yoffset)>& fn)
{
	_onMouseWheelEvent = fn;
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

const Input& Window::input() const
{
	return _input;
}

const bool Window::is_open() const 
{
	return !glfwWindowShouldClose(_window);
}

void Window::tick()
{
	glfwPollEvents();
}

void Window::on_keyboard_message_forwarder(GLFWwindow* glfwWindow, int key, int scancode, int action, int modifiers)
{
	Window& window = *Window::_glfwWindowToWindowMappingForStaticCallbacks[glfwWindow];
	if (!window._onKeyEvent) {
		return;
	}

	const Input::PressableState state = static_cast<Input::PressableState>(action);
	const Input::Pressable terminal = static_cast<Input::Pressable>(key);
	
	window._onKeyEvent(terminal, state, modifiers);
}

void Window::on_mouse_position_message_forwarder(GLFWwindow* glfwWindow, double x, double y)
{
	Window& window = *Window::_glfwWindowToWindowMappingForStaticCallbacks[glfwWindow];
	window._input.update_mouse_movement();
	if (!window._onMouseMoveEvent) {
		return;
	}

	window._onMouseMoveEvent(x, y);
}

void Window::on_mouse_button_message_forwarder(GLFWwindow* glfwWindow, int button, int action, int modifiers)
{
	Window& window = *Window::_glfwWindowToWindowMappingForStaticCallbacks[glfwWindow];
	if (!window._onMouseButtonEvent) {
		return;
	}

	const Input::PressableState state = static_cast<Input::PressableState>(action);
	const Input::Pressable terminal = static_cast<Input::Pressable>(button + static_cast<int>(Input::Pressable::MOUSE_BUTTON_1));
	window._onMouseButtonEvent(terminal, state, modifiers);
}

void Window::on_mouse_scroll_wheel_message_forwarder(GLFWwindow* glfwWindow, double xoffset, double yoffset)
{
	Window& window = *Window::_glfwWindowToWindowMappingForStaticCallbacks[glfwWindow];
	if (!window._onMouseWheelEvent) {
		return;
	}

	window._onMouseWheelEvent(xoffset, yoffset);
}

std::unordered_map<GLFWwindow*, Window*> Window::_glfwWindowToWindowMappingForStaticCallbacks = std::unordered_map<GLFWwindow*, Window*>();