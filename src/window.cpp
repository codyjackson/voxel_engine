#include "window.h"

Window::Window()
{
	if(!glfwInit())
		throw std::runtime_error("Failed to initialize glfw.");
	if(!(_window = glfwCreateWindow(1024, 768, "", NULL, NULL)))
		throw std::runtime_error("Failed to create window.");
	glfwHideWindow(_window);

	//Remember to clear callbacks in the destructor
	glfwSetKeyCallback(_window, Window::on_keyboard_message_forwarder);
	_glfwWindowToWindowMappingForStaticCallbacks.emplace(_window, this);
}

Window::~Window()
{
	if (!_window)
		return;

	glfwSetKeyCallback(_window, nullptr);
	_glfwWindowToWindowMappingForStaticCallbacks.erase(_window);

	glfwDestroyWindow(_window);
	glfwTerminate();
}

void Window::open()
{
	glfwShowWindow(_window);
	glfwMakeContextCurrent(_window);

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void Window::draw()
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

void Window::update_width(const int newWidth)
{
	glfwSetWindowSize(_window, newWidth, get_height());
}

void Window::update_height(const int newHeight)
{
	glfwSetWindowSize(_window, get_width(), newHeight);
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

Input& Window::input()
{
	return _input;
}

const bool Window::is_open() const 
{
	return !glfwWindowShouldClose(_window);
}

void Window::poll_events() const
{
	glfwPollEvents();
}

void Window::on_keyboard_message_forwarder(GLFWwindow* glfwWindow, int key, int scancode, int action, int modifiers)
{
	if (action == GLFW_REPEAT)
		return;

	Window& window = *Window::_glfwWindowToWindowMappingForStaticCallbacks[glfwWindow];
	const Input::PressableState state = static_cast<Input::PressableState>(action);
	const Input::Pressable terminal = static_cast<Input::Pressable>(key);
	window._input.update(terminal, state);
}

std::unordered_map<GLFWwindow*, Window*> Window::_glfwWindowToWindowMappingForStaticCallbacks = std::unordered_map<GLFWwindow*, Window*>();