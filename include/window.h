#pragma once

#include <GLFW\glfw3.h>
#include <iostream>
#include <string>

#define DEFAULT_HEIGHT 480
#define DEFAULT_WIDTH 640

class Window
{
	public:
		Window();
		~Window();
		void init();
		void swapBuffer();
		void update();
		void clear();

	private:
		GLFWwindow* window;
};