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
	glfwMakeContextCurrent(window);

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void Window::draw(const Bitmap& scene)
{
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glRasterPos2f(-1.0f, -1.0f);
	glDrawPixels(scene.get_width(), scene.get_height(), GL_RGB, GL_UNSIGNED_BYTE, scene.get_pixels());
	glfwSwapBuffers(window);
}


void Window::close()
{
	glfwSetWindowShouldClose(window,GL_TRUE);
	glfwHideWindow(window);
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