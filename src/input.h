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
	enum class Pressable
	{
		SPACE = 32,
		APOSTROPHE = 39,
		COMMA = 44,
		MINUS = 45,
		PERIOD = 46,
		SLASH = 47,
		NUM_0 = 48,
		NUM_1 = 49,
		NUM_2 = 50,
		NUM_3 = 51,
		NUM_4 = 52,
		NUM_5 = 53,
		NUM_6 = 54,
		NUM_7 = 55,
		NUM_8 = 56,
		NUM_9 = 57,
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
		LEFT_SHIFT = 340,
		LEFT_CONTROL = 341,
		LEFT_ALT = 342,
		LEFT_SUPER = 343,
		RIGHT_SHIFT = 344,
		RIGHT_CONTROL = 345,
		RIGHT_ALT = 346,
		RIGHT_SUPER = 347,
		MENU = 348,
		MOUSE_BUTTON_1,
		MOUSE_BUTTON_2,
		MOUSE_BUTTON_3,
		MOUSE_BUTTON_4,
		MOUSE_BUTTON_5,
		MOUSE_BUTTON_6,
		NONE
	};

	enum class PressableEvents
	{
		ON_PRESSED,
		WHILE_PRESSED,
		ON_RELEASED,
		WHILE_RELEASED
	};

	enum class Moveable
	{
		MOUSE,
		MOUSE_WHEEL
	};

	template<typename TERMINAL_TYPE>
	class Combo
	{
	public:
		struct Hasher
		{ 
			size_t operator()(const Combo& s) const
			{
				const auto hashAccumulateModifier = [](const size_t& l, const Pressable& r) {return l ^ std::hash<size_t>()(static_cast<size_t>(r)); };
				const size_t modifiersHash = std::accumulate(std::begin(s._modifiers), std::end(s._modifiers), static_cast<size_t>(0), hashAccumulateModifier);
				const size_t terminalHash = std::hash<size_t>()(static_cast<size_t>(s._terminal));

				return modifiersHash ^ terminalHash;
			}
		};

		Combo(Pressable mod0, Pressable mod1, Pressable mod2, TERMINAL_TYPE terminal)
			:_modifiers{ { mod0, mod1, mod2 } }, _terminal(terminal)
		{
			//Necessary for easier equality tests since the order of the modifiers doesn't matter.
			std::sort(std::begin(_modifiers), std::end(_modifiers));
		}

		Combo(Pressable mod0, Pressable mod1, TERMINAL_TYPE terminal)
			: Combo(mod0, mod1, Pressable::NONE, terminal)
		{}

		Combo(Pressable mod0, TERMINAL_TYPE terminal)
			: Combo(mod0, Pressable::NONE, terminal)
		{}

		Combo(TERMINAL_TYPE terminal)
			: Combo(Pressable::NONE, terminal)
		{}

		bool operator==(const Combo& rhs) const
		{
			return (this->_terminal == rhs._terminal) && (this->_modifiers == rhs._modifiers);
		}

		TERMINAL_TYPE get_terminal() const
		{
			return _terminal;
		}

		const std::array<Pressable, 3>& get_modifiers() const
		{
			return _modifiers;
		}

	private:
		std::array<Pressable, 3> _modifiers;
		TERMINAL_TYPE _terminal;
	};

	typedef Combo<Pressable> PressableCombo;
	typedef Combo<Moveable> MoveableCombo;

	class Mouse
	{
	public: 
		void lock_movement();
		void unlock_movement();
		bool is_movement_locked() const;

		glm::ivec2 get_position() const;
		glm::ivec2 get_position_delta() const;
		int get_wheel_delta() const;

	private:
		friend class Input;
		Mouse();

		void update_position(const glm::ivec2& position);
		void update_locked_position(const glm::ivec2& lockedPosition, const glm::ivec2& movedPosition);

		glm::ivec2 _position;
		glm::ivec2 _oldPosition;
		int _wheelDelta;
		bool _isMovementLocked;
	};

	Mouse& mouse();
	void on(const PressableCombo& combo, const std::function<void(Input&)>& callback);
	void on(const MoveableCombo& combo, const std::function<void(Input&)>& callback);

private:
	friend class Window;

	enum class PressableState
	{
		UP,
		DOWN
	};

	void prepare_for_updates();
	void update(Pressable terminal, PressableState state);
	void update_mouse_locked_position(const glm::ivec2& lockedPosition, const glm::ivec2& movedPosition);
	void update_mouse_position(const glm::ivec2& xy);
	void update_mouse_scroll_wheel(int clicks);

	void invoke_bound_callback(const PressableCombo& combo);
	void invoke_bound_callback(const MoveableCombo& combo);

	void signal_key_pressed(Pressable t);
	void signal_moveable(Moveable m);

	bool is_pressable_pressed(Pressable p) const;
	bool are_modifiers_pressed(const std::array<Pressable, 3>& modifiers) const;

	Mouse _mouse;

	std::unordered_map<Pressable, std::vector<PressableCombo>> _pressableTerminalToCombos;
	std::unordered_map<Moveable, std::vector<MoveableCombo>> _moveableTerminalToCombos;

	std::unordered_map<PressableCombo, std::function<void(Input&)>, PressableCombo::Hasher> _pressableComboToCallback;
	std::unordered_map<MoveableCombo, std::function<void(Input&)>, MoveableCombo::Hasher> _moveableComboToCallback;

	std::unordered_map<Pressable, PressableState> _pressableToKeyState;
};