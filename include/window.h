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
		Window(std::string);
		Window(int.int);
		Window(int, int, std::string);
		~Window();
		void open_window();
		void swap_buffer();
		void update();
		void clear();
		void close_window();
		void set_width(int);
		void set_height(int);
		static int get_width();
		static int get_height();
	
	private:
		GLFWwindow* window;
		bool openedWindow;
		bool fullscreen;
};