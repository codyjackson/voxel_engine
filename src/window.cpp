#include "window.h"

#include <iostream>

const int DEFAULT_WIDTH = 480;
const int DEFAULT_HEIGHT = 640;

Window::Window()
{
	if(!glfwInit())
		std::cout<<"Error: glfw not initilized"<<std::endl;
	window = glfwCreateWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, "window", NULL, NULL);
	if(!window)
		std::cout<<"Error: glfw window not open"<<std::endl;
	glfwHideWindow(window);
}

Window::Window(const std::string& title )
{
	if(!glfwInit())
		std::cout<<"Error: glfw not initilized"<<std::endl;
	window = glfwCreateWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, title.c_str(), NULL, NULL);
	if(!window)
		std::cout<<"Error: glfw window not open"<<std::endl;
	glfwHideWindow(window);
}
Window::Window(const int width, const int height)
{
	if(!glfwInit())
		std::cout<<"Error: glfw not initilized"<<std::endl;
	window = glfwCreateWindow(width, height, "window", NULL, NULL);
	if(!window)
		std::cout<<"Error: glfw window not open"<<std::endl;
	glfwHideWindow(window);
}
Window::Window(const int width, const int height, const std::string& title)
{
	if(!glfwInit())
		std::cout<<"Error: glfw not initilized"<<std::endl;
	window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if(!window)
		std::cout<<"Error: glfw window not open"<<std::endl;
	glfwHideWindow(window);
}

Window::~Window()
{
	glfwDestroyWindow(window);
}

void Window::open()
{
	glfwShowWindow(window);
}

void Window::draw(const Bitmap& scene)
{
	glfwMakeContextCurrent(window);
}

void Window::hide()
{
	glfwHideWindow(window);
}

void Window::close()
{
	glfwSetWindowShouldClose(window,GL_TRUE);
	hide();
}

void Window::update_width(const int& newWidth)
{
	glfwSetWindowSize(window, newWidth, get_height());
}

void Window::update_height(const int& newHeight)
{
	glfwSetWindowSize(window, get_width(), newHeight);
}

const int Window::get_width() const
{
	int* width;
	int* trash;
	glfwGetWindowSize(window, width, trash);
	return *width;
}

const int Window::get_height() const
{
	int* height;
	int* trash;
	glfwGetWindowSize(window, trash, height);
	return *height;
}

const bool Window::is_open() const 
{
	if(glfwWindowShouldClose(window))
		return false;
	return true;
}