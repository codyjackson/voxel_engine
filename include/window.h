#pragma once

#include <string>
#include <GLFW\glfw3.h>
#include "bitmap.h"

class Window
{
	public:
		~Window();

		void draw();

		void close();

		void update_title(const std::string& title);

		void update_width(const int newWidth);
		void update_height(const int newHeight);

		const int get_width() const;
		const int get_height() const;
		
	private:
		friend class MainLoop;
		Window();

		void open();
		const bool is_open() const;

		void poll_events() const;

		GLFWwindow* _window;
};