#pragma once

#include <string>
#include <GLFW\glfw3.h>
class Bitmap; //to be replaced with actual class header

class Window
{
	public:
		Window();
		Window(const std::string& title );
		Window(const int& width, const int& height);
		Window(const int& width, const int& height, const std::string& title);
		~Window();
		void open();
		void draw(const Bitmap& scene);
		void close();
		void update_width(int);
		void update_height(int);
		const int get_width() const;
		const int get_height() const;
	
	private:
		GLFWwindow* window;
		bool openedWindow;
		bool fullscreen;
};