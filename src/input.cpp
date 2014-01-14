#include "input.h"
#include "window.h"

#include <algorithm>

Input::Mouse::Mouse(const glm::ivec2& position, const glm::ivec2& positionDelta, int wheelDelta)
:_position(position), _positionDelta(positionDelta), _wheelDelta(wheelDelta)
{}

glm::ivec2 Input::Mouse::get_position() const
{
	return _position;
}

glm::ivec2 Input::Mouse::get_position_delta() const
{
	return _positionDelta;
}

int Input::Mouse::get_wheel_delta() const
{
	return _wheelDelta;
}

Input::KeyboardSequence::KeyboardSequence(Modifier m1, Modifier m2, Modifier m3, KeyboardTerminal terminal)
:_control(contains_specified_modifier<Modifier::CTRL>(m1, m2, m3)),  
 _alt(contains_specified_modifier<Modifier::ALT>(m1, m2, m3)),
 _shift(contains_specified_modifier<Modifier::SHIFT>(m1, m2, m3)),
 _terminal(terminal)
{}

bool Input::KeyboardSequence::operator == (const KeyboardSequence& rhs) const
{
	return (this->_control == rhs._control) && (this->_alt == rhs._alt) && (this->_shift == rhs._shift) && (this->_terminal == rhs._terminal); 
}

size_t Input::KeyboardSequence::Hasher::operator()(const KeyboardSequence& s) const 
{
	const size_t altHash = static_cast<bool>(s._alt);
	const size_t controlHash = static_cast<bool>(s._control);
	const size_t shiftHash = static_cast<bool>(s._shift);
	const size_t terminalHash = std::hash<int>()(static_cast<int>(s._terminal));
	return altHash ^ controlHash ^ shiftHash ^ terminalHash;
}

void Input::on(const KeyboardSequence& sequence, const std::function<void()>& callback)
{
	_keyboardSequenceToCallback[sequence] = callback;
}


bool Input::is_modifier_down(Modifier m) const
{
	const auto keyState = _modifierToKeyState.find(m);
	if (keyState == _modifierToKeyState.end())
		return false;
	return keyState->second == KeyState::DOWN;
}

void Input::signal_key_pressed(KeyboardTerminal t) const
{
	const Modifier m1 = is_modifier_down(Modifier::CTRL) ? Modifier::CTRL : Modifier::NONE;
	const Modifier m2 = is_modifier_down(Modifier::SHIFT) ? Modifier::SHIFT : Modifier::NONE;
	const Modifier m3 = is_modifier_down(Modifier::ALT) ? Modifier::ALT : Modifier::NONE;
	const KeyboardSequence sequence = KeyboardSequence(m1, m2, m3, t);
	
	const auto sequenceToCallbackIter = _keyboardSequenceToCallback.find(sequence);
	if (sequenceToCallbackIter != _keyboardSequenceToCallback.end())
		sequenceToCallbackIter->second();
}

void Input::update(KeyboardTerminal terminal, KeyState state)
{
	if (_keyboardTerminalToKeyState.find(terminal) == _keyboardTerminalToKeyState.end())
		_keyboardTerminalToKeyState[terminal] = KeyState::UP;

	KeyState& keyState = _keyboardTerminalToKeyState[terminal];
	const KeyState oldKeyState = keyState;
	keyState = state;

	if (oldKeyState == KeyState::UP && keyState == KeyState::DOWN)
		signal_key_pressed(terminal);
}

void Input::update(Modifier modifier, KeyState state)
{
	if (modifier == Modifier::NONE)
		return;

	_modifierToKeyState[modifier] = state;
}

 Input::Modifier Input::convert_to_modifier(int modifier)
{
	 switch (modifier)
	 {
	 case GLFW_KEY_LEFT_ALT:
	 case GLFW_KEY_RIGHT_ALT:
		 return Modifier::ALT;
	 case GLFW_KEY_LEFT_CONTROL:
	 case GLFW_KEY_RIGHT_CONTROL:
		 return Modifier::CTRL;
	 case GLFW_KEY_LEFT_SHIFT:
	 case GLFW_KEY_RIGHT_SHIFT:
		 return Modifier::SHIFT;
	 default:
		 return Modifier::NONE;
	 }
}