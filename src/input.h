#pragma once

#include <array>
#include <functional>
#include <unordered_map>

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Window;

class Input
{
public:
	enum class Modifier 
	{
		CTRL,
		SHIFT,
		ALT,
		NONE
	};

	enum class KeyboardTerminal
	{
		SPACE = 32,
		APOSTROPHE = 39,
		COMMA = 44,
		MINUS = 45,
		PERIOD = 46,
		SLASH = 47,
		KEY_0 = 48,
		KEY_1 = 49,
		KEY_2 = 50,
		KEY_3 = 51,
		KEY_4 = 52,
		KEY_5 = 53,
		KEY_6 = 54,
		KEY_7 = 55,
		KEY_8 = 56,
		KEY_9 = 57,
		SEMICOLON = 59,
		EQUAL = 61,
		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,
		LEFT_BRACKET = 91,
		BACKSLASH = 92,
		RIGHT_BRACKET = 93,
		GRAVE_ACCENT = 96,
		WORLD_1 = 61,
		WORLD_2 = 62,
		ESCAPE = 256,
		ENTER = 257,
		TAB = 258,
		BACKSPACE = 259,
		INSERT = 260,
		DELETE = 261,
		RIGHT = 262,
		LEFT = 263,
		DOWN = 264,
		UP = 265,
		PAGE_UP = 266,
		PAGE_DOWN = 267,
		HOME = 268,
		END = 269,
		CAPS_LOCK = 280,
		SCROLL_LOCK = 281,
		NUM_LOCK = 282,
		PRINT_SCREEN = 283,
		PAUSE = 284,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,
		KP_0 = 320,
		KP_1 = 321,
		KP_2 = 322,
		KP_3 = 323,
		KP_4 = 324,
		KP_5 = 325,
		KP_6 = 326,
		KP_7 = 327,
		KP_8 = 328,
		KP_9 = 329,
		KP_DECIMAL = 330,
		KP_DIVIDE = 331,
		KP_MULTIPLY = 332,
		KP_SUBTRACT = 333,
		KP_ADD = 334,
		KP_ENTER = 335,
		KP_EQUAL = 336,
		MENU = 348
	};

	enum class KeyState
	{
		UP,
		DOWN
	};

	enum class MouseTerminal
	{
	};


	class KeyboardSequence
	{
	public:
		struct Hasher{ size_t operator()(const KeyboardSequence& s) const; };

		KeyboardSequence(Modifier m1, Modifier m2, Modifier m3, KeyboardTerminal terminal);
		bool operator==(const KeyboardSequence& rhs) const;

	private:
		//I'm templating this function to make it less error prone. 
		//(ex: accidently passing the expected value to m1 instead of the expected value parameter.
		template<Modifier M>
		bool contains_specified_modifier(Modifier m1, Modifier m2, Modifier m3)
		{
			return (m1 == M) || (m2 == M) || (m3 == M);
		}

		bool _control;
		bool _alt;
		bool _shift;
		KeyboardTerminal _terminal;
	};

	class MouseSequence
	{

	};

	class Mouse
	{
	public: 
		Mouse(const glm::ivec2& position, const glm::ivec2& positionDelta, int wheelDelta);
		glm::ivec2 get_position() const;
		glm::ivec2 get_position_delta() const;
		int get_wheel_delta() const;

	private:
		const glm::ivec2 _position;
		const glm::ivec2 _positionDelta;
		const int _wheelDelta;
	};

	void on(const KeyboardSequence& sequence, const std::function<void()>& callback);
	void on(const MouseSequence& macro, const std::function<void(const MouseSequence& origin, const Mouse& mouse)>& callback);

	void signal_key_pressed(KeyboardTerminal t) const;
	void update(KeyboardTerminal terminal, KeyState state);
	void update(Modifier modifier, KeyState state);

	static Modifier convert_to_modifier(int modifier);

private:
	bool is_modifier_down(Modifier m) const;

	std::unordered_map<Modifier, KeyState> _modifierToKeyState;
	std::unordered_map<KeyboardTerminal, KeyState> _keyboardTerminalToKeyState;
 	std::unordered_map<KeyboardSequence, std::function<void()>, KeyboardSequence::Hasher> _keyboardSequenceToCallback;
};