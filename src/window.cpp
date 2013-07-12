#include "window.h"

Window::Window()
{
	if(!glfwInit())
		throw std::runtime_error("Failed to initialize glfw.");
	if(!(window = glfwCreateWindow(1, 1, "", NULL, NULL)))
		throw std::runtime_error("Failed to create window.");
	glfwHideWindow(window);
}

Window::~Window()
{
	if(window)
		glfwDestroyWindow(window);
	glfwTerminate();
}

void Window::open()
{
	glfwShowWindow(window);
	glfwMakeContextCurrent(window);

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void Window::draw(const Bitmap& scene)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glRasterPos2f(-1.0f, -1.0f);
	glDrawPixels(scene.get_width(), scene.get_height(), GL_RGB, GL_UNSIGNED_BYTE, scene.get_pixels().data());
	glfwSwapBuffers(window);
}


void Window::close()
{
	glfwSetWindowShouldClose(window,GL_TRUE);
	glfwHideWindow(window);
}

void Window::update_title(const std::string& title)
{
	glfwSetWindowTitle(window, title.c_str());
}

void Window::update_width(const int newWidth)
{
	glfwSetWindowSize(window, newWidth, get_height());
}

void Window::update_height(const int newHeight)
{
	glfwSetWindowSize(window, get_width(), newHeight);
}

const int Window::get_width() const
{
	int width;
	int trash;
	glfwGetWindowSize(window, &width, &trash);
	return width;
}

const int Window::get_height() const
{
	int height;
	int trash;
	glfwGetWindowSize(window, &trash, &height);
	return height;
}

const bool Window::is_open() const 
{
	return !glfwWindowShouldClose(window);
}

void Window::poll_events() const
{
	glfwPollEvents();
}