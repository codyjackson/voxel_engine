#pragma once

#include <string>
#include <GLFW\glfw3.h>
class Bitmap; //to be replaced with actual class header

class Window
{
	public:
		Window();
		Window(const std::string& title );
		Window(const int width, const int height);
		Window(const int width, const int height, const std::string& title);
		~Window();
		void open();
		void draw(const Bitmap& scene);
		void close();
		void update_width(const int newWidth);
		void update_height(const int newHeight);
		const int get_width() const;
		const int get_height() const;
		const bool is_open() const;
	private:
		GLFWwindow* window;
};