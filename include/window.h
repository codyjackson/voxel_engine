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
		void hide();
		void update_width(const int& newWidth);
		void update_height(const int& newHeight);
		const int get_width() const;
		const int get_height() const;
		bool is_opened();
	private:
		GLFWwindow* window;
		bool openWindow;
		bool fullscreen;
		int width;
		int height;
};