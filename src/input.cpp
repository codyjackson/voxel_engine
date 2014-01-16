#include "input.h"
#include "window.h"

#include <algorithm>
#include <numeric>

Input::Mouse::Mouse()
{}

glm::ivec2 Input::Mouse::get_position() const
{
	return _position;
}

glm::ivec2 Input::Mouse::get_position_delta() const
{
	return _position - _oldPosition;
}

int Input::Mouse::get_wheel_delta() const
{
	return _wheelDelta;
}

void Input::Mouse::update_position(const glm::ivec2& position)
{
	_oldPosition = _position;
	_position = position;
}

void Input::invoke_bound_callback(const PressableCombo& combo)
{
	const auto iter = _pressableComboToCallback.find(combo);
	if (iter != _pressableComboToCallback.end())
		iter->second(*this);
}

void Input::invoke_bound_callback(const MoveableCombo& combo)
{
	const auto iter = _moveableComboToCallback.find(combo);
	if (iter != _moveableComboToCallback.end())
		iter->second(*this);
}

void Input::signal_key_pressed(Pressable t)
{
	const auto combosIter = _pressableTerminalToCombos.find(t);
	if (combosIter == _pressableTerminalToCombos.end())
		return;

	const auto isActionable = [this](const PressableCombo& c){ 
		return is_pressable_pressed(c.get_terminal()) && are_modifiers_pressed(c.get_modifiers());
	};

	const auto invokeBoundCallback = std::bind(static_cast<void(Input::*)(const PressableCombo&)>(&Input::invoke_bound_callback), this, std::placeholders::_1);
	const std::vector<PressableCombo>& potentialCombos = combosIter->second;
	std::vector<PressableCombo> actionableCombos;
	std::copy_if(std::begin(potentialCombos), std::end(potentialCombos), std::back_inserter(actionableCombos), isActionable);
	std::for_each(std::begin(actionableCombos), std::end(actionableCombos), invokeBoundCallback);
}

void Input::signal_moveable(Moveable m)
{
	const auto combosIter = _moveableTerminalToCombos.find(m);
	if (combosIter == _moveableTerminalToCombos.end())
		return;

	const auto isActionable = [this](const MoveableCombo& c){
		return are_modifiers_pressed(c.get_modifiers());
	};
	const auto invokeBoundCallback = std::bind(static_cast<void(Input::*)(const MoveableCombo&)>(&Input::invoke_bound_callback), this, std::placeholders::_1);
	const std::vector<MoveableCombo>& potentialCombos = combosIter->second;
	std::vector<MoveableCombo> actionableCombos;
	std::copy_if(std::begin(potentialCombos), std::end(potentialCombos), std::back_inserter(actionableCombos), isActionable);
	std::for_each(std::begin(actionableCombos), std::end(actionableCombos), invokeBoundCallback);
}

const Input::Mouse& Input::mouse() const
{
	return _mouse;
}

void Input::on(const PressableCombo& combo, const std::function<void(Input&)>& callback)
{
	_pressableComboToCallback[combo] = callback;
	_pressableTerminalToCombos[combo.get_terminal()].push_back(combo);
}

void Input::on(const MoveableCombo& combo, const std::function<void(Input&)>& callback)
{
	_moveableComboToCallback[combo] = callback;
	_moveableTerminalToCombos[combo.get_terminal()].push_back(combo);
}

void Input::prepare_for_updates()
{
	_mouse._wheelDelta = 0;
}

void Input::update(Pressable terminal, PressableState state)
{
	if (_pressableToKeyState.find(terminal) == _pressableToKeyState.end())
		_pressableToKeyState[terminal] = PressableState::UP;

	PressableState& keyState = _pressableToKeyState[terminal];
	const PressableState oldKeyState = keyState;
	keyState = state;

	if (oldKeyState == PressableState::UP && keyState == PressableState::DOWN)
		signal_key_pressed(terminal);
}

void Input::update_mouse_position(const glm::ivec2& xy)
{
	_mouse._position = xy;
	signal_moveable(Input::Moveable::MOUSE);
}

void Input::update_mouse_scroll_wheel(int clicks)
{
	_mouse._wheelDelta = clicks;
	signal_moveable(Input::Moveable::MOUSE_WHEEL);
}

bool Input::is_pressable_pressed(Pressable p) const
{
	if (p == Pressable::NONE)
		return true;

	const auto iter = _pressableToKeyState.find(p);
	if (iter == _pressableToKeyState.end())
		return false;

	return iter->second == PressableState::DOWN;
}

bool Input::are_modifiers_pressed(const std::array<Pressable, 3>& modifiers) const
{
	const auto boundIsPressablePressed = std::bind(&Input::is_pressable_pressed, this, std::placeholders::_1);
	return std::all_of(std::begin(modifiers), std::end(modifiers), boundIsPressablePressed);
}