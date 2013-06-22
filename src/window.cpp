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
}

Window::Window(const std::string& title )
{
	if(!glfwInit())
		std::cout<<"Error: glfw not initilized"<<std::endl;
	window = glfwCreateWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, title.c_str(), NULL, NULL);
	if(!window)
		std::cout<<"Error: glfw window not open"<<std::endl;
}
Window::Window(const int& width, const int& height)
{
	if(!glfwInit())
		std::cout<<"Error: glfw not initilized"<<std::endl;
	window = glfwCreateWindow(width, height, "window", NULL, NULL);
	if(!window)
		std::cout<<"Error: glfw window not open"<<std::endl;
}
Window::Window(const int& width, const int& height, const std::string& title)
{
	if(!glfwInit())
		std::cout<<"Error: glfw not initilized"<<std::endl;
	window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if(!window)
		std::cout<<"Error: glfw window not open"<<std::endl;
}

Window::~Window()
{
	glfwDestroyWindow(window);
    glfwTerminate();
}

void Window::open()
{
	glfwMakeContextCurrent(window);
}