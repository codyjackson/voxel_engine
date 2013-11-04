#include "window.h"

Window::Window()
{
	if(!glfwInit())
		throw std::runtime_error("Failed to initialize glfw.");
	if(!(_window = glfwCreateWindow(1024, 768, "", NULL, NULL)))
		throw std::runtime_error("Failed to create window.");
	glfwHideWindow(_window);
}

Window::~Window()
{
	if(_window)
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
	glfwSetWindowShouldClose(_window,GL_TRUE);
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

const bool Window::is_open() const 
{
	return !glfwWindowShouldClose(_window);
}

void Window::poll_events() const
{
	glfwPollEvents();
}