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
	fullscreen = false;
	openWindow = false;
	this->width =	DEFAULT_WIDTH;
	this->height = DEFAULT_HEIGHT;
	glfwHideWindow(window);
}

Window::Window(const std::string& title )
{
	if(!glfwInit())
		std::cout<<"Error: glfw not initilized"<<std::endl;
	window = glfwCreateWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, title.c_str(), NULL, NULL);
	if(!window)
		std::cout<<"Error: glfw window not open"<<std::endl;
	fullscreen = false;
	openWindow = false;
	this->width = DEFAULT_WIDTH;
	this->height = DEFAULT_HEIGHT;
	glfwHideWindow(window);
}
Window::Window(const int& width, const int& height)
{
	if(!glfwInit())
		std::cout<<"Error: glfw not initilized"<<std::endl;
	window = glfwCreateWindow(width, height, "window", NULL, NULL);
	if(!window)
		std::cout<<"Error: glfw window not open"<<std::endl;
	fullscreen = false;
	openWindow = false;
	this->width = width;
	this->height = height;
	glfwHideWindow(window);
}
Window::Window(const int& width, const int& height, const std::string& title)
{
	if(!glfwInit())
		std::cout<<"Error: glfw not initilized"<<std::endl;
	window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if(!window)
		std::cout<<"Error: glfw window not open"<<std::endl;
	fullscreen = false;
	openWindow = false;
	this->width = width;
	this->height = height;
	glfwHideWindow(window);
}

Window::~Window()
{
	glfwDestroyWindow(window);
    glfwTerminate();
}

void Window::open()
{
	openWindow = true;
	glfwMakeContextCurrent(window);
	glfwShowWindow(window);
}

void Window::draw(const Bitmap& scene)
{

}

void Window::hide()
{
	glfwHideWindow(window);
}
void Window::update_width(const int& newWidth)
{
	this->width = newWidth;
	glfwSetWindowSize(window, newWidth, height);
}
void Window::update_height(const int& newHeight)
{
	this->height = newHeight;
	glfwSetWindowSize(window, width, newHeight);
}
const int Window::get_width() const
{
	return width;
}
const int Window::get_height() const
{
	return height;
}

bool Window::is_opened() 
{
	if(glfwWindowShouldClose(window))
		openWindow = false;
	return openWindow;
}