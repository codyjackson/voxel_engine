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
		const glm::ivec2 get_center() const;

		Input& input();
		
	private:
		friend class MainLoop;
		Window();

		static void on_keyboard_message_forwarder(GLFWwindow* glfwWindow, int key, int scancode, int action, int modifiers);
		static void on_mouse_position_message_forwarder(GLFWwindow* glfwWindow, double x, double y);
		static void on_mouse_button_message_forwarder(GLFWwindow* glfwWindow, int button, int action, int modifiers);
		static void on_mouse_scroll_wheel_message_forwarder(GLFWwindow* glfwWindow, double xoffset, double yoffset);

		void open();
		const bool is_open() const;
		void tick();

		static std::unordered_map<GLFWwindow*, Window*> _glfwWindowToWindowMappingForStaticCallbacks;
		Input _input;
		GLFWwindow* _window;
};