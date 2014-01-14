#pragma once

#include "input.h"

#include <string>
#include <unordered_map>
#include <GLFW\glfw3.h>

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

		Input& input();

	private:
		friend class MainLoop;
		Window();

		static void on_keyboard_message_forwarder(GLFWwindow* window, int key, int scancode, int action, int modifiers);

		void open();
		const bool is_open() const;
		void poll_events() const;

		static std::unordered_map<GLFWwindow*, Window*> _glfwWindowToWindowMappingForStaticCallbacks;
		Input _input;
		GLFWwindow* _window;
};