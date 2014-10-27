#pragma once
#include <Windows.h>
#include "input.h"
#include "ui.h"

#include <string>
#include <unordered_map>

#include <GLFW/glfw3.h>

class Window
{
	public:
		~Window();

		void render();
		void close();

		void update_title(const std::string& title);
		void update_resolution(const RectSize& resolution);

		void on_key_event(const std::function<void(Input::Pressable key, Input::PressableState state, int modifiers)>& fn);
		void on_mouse_button_event(const std::function<void(Input::Pressable button, Input::PressableState state, int modifiers)>& fn);
		void on_mouse_move_event(const std::function<void(double x, double y)>& fn);
		void on_mouse_wheel_event(const std::function<void(double xoffset, double yoffset)>& fn);

		RectSize get_resolution() const;
		const int get_width() const;
		const int get_height() const;
		const glm::ivec2 get_center() const;
		
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

		GLFWwindow* _window;
		std::function<void(Input::Pressable key, Input::PressableState state, int modifiers)> _onKeyEvent;
		std::function<void(Input::Pressable button, Input::PressableState state, int modifiers)> _onMouseButtonEvent;
		std::function<void(double x, double y)> _onMouseMoveEvent;
		std::function<void(double xoffset, double yoffset)> _onMouseWheelEvent;
};