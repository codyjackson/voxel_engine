#include "window.h"

Window::Window()
{
	if(!glfwInit())
		std::cout<<"Error: glfw not initilized"<<std::endl;
	window = glfwCreateWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, "window", NULL, NULL);
	if(!window)
		std::cout<<"Error: glfw window not open"<<std::endl;
}

Window::~Window()
{
	glfwDestroyWindow(window);
    glfwTerminate();
}

void Window::open_window()
{
	glfwMakeContextCurrent(window);
}

void Window::swap_buffer()
{
	glfwSwapBuffers(window);
}

void Window::update()
{
	glfwPollEvents();
}

void Window::clear()
{
	glClear( GL_COLOR_BUFFER_BIT );
}