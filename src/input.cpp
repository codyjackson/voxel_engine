#include "input.h"
#include "window.h"

#include <algorithm>
#include <numeric>

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

void Input::call_bound_callback(const Combo& combo) const
{
	const auto iter = _comboToCallback.find(combo);
	if (iter != _comboToCallback.end())
		iter->second();
}

void Input::signal_key_pressed(Pressable t) const
{
	const auto combosIter = _pressableTerminalToCombos.find(t);
	if (combosIter == _pressableTerminalToCombos.end())
		return;

	const auto isActionable = [this](const Combo& c){ 
		return is_pressable_pressed(c.get_pressable_terminal()) && are_modifiers_pressed(c.get_modifiers());
	};

	const std::vector<Combo>& potentialCombos = combosIter->second;
	std::vector<Combo> actionableCombos;
	std::copy_if(std::begin(potentialCombos), std::end(potentialCombos), std::back_inserter(actionableCombos), isActionable);
	std::for_each(std::begin(actionableCombos), std::end(actionableCombos), std::bind(&Input::call_bound_callback, this, std::placeholders::_1));
}

void Input::on(const Combo& combo, const std::function<void()>& callback)
{
	_comboToCallback[combo] = callback;
	if (combo.is_terminal_pressable())
		_pressableTerminalToCombos[combo.get_pressable_terminal()].push_back(combo);
	else
		_moveableTerminalToCombos[combo.get_moveable_terminal()].push_back(combo);
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

size_t Input::Combo::Hasher::operator()(const Combo& s) const
{
	const auto hashAccumulateModifier = [](const size_t& l, const Pressable& r) {return l ^ std::hash<size_t>()(static_cast<size_t>(r)); };
	const size_t modifiersHash = std::accumulate(std::begin(s._modifiers), std::end(s._modifiers), static_cast<size_t>(0), hashAccumulateModifier);
	const size_t pressableTerminalHash = std::hash<size_t>()(static_cast<size_t>(s._pressableTerminal));
	const size_t moveableTerminalHash = std::hash<size_t>()(static_cast<size_t>(s._pressableTerminal));

	return modifiersHash ^ pressableTerminalHash ^ moveableTerminalHash;
}

Input::Combo::Combo(Pressable mod0, Pressable mod1, Pressable mod2, Pressable terminal)
:_modifiers{ { mod0, mod1, mod2 } }, _pressableTerminal(terminal), _moveableTerminal(Moveable::NONE)
{
	//Necessary for easier equality tests
	std::sort(std::begin(_modifiers), std::end(_modifiers));
}

Input::Combo::Combo(Pressable mod0, Pressable mod1, Pressable terminal)
: Combo(mod0, mod1, Pressable::NONE, terminal)
{}

Input::Combo::Combo(Pressable mod0, Pressable terminal)
: Combo(mod0, Pressable::NONE, terminal)
{}

Input::Combo::Combo(Pressable terminal)
: Combo(Pressable::NONE, terminal)
{}

Input::Combo::Combo(Pressable mod0, Pressable mod1, Pressable mod2, Moveable terminal)
: _modifiers{ { mod0, mod1, mod2 } }, _pressableTerminal(Pressable::NONE), _moveableTerminal(terminal)
{
	//Necessary for easier equality tests
	std::sort(std::begin(_modifiers), std::end(_modifiers));
}

Input::Combo::Combo(Pressable mod0, Pressable mod1, Moveable terminal)
: Combo(mod0, mod1, Pressable::NONE, terminal)
{}

Input::Combo::Combo(Pressable mod0, Moveable terminal)
: Combo(mod0, Pressable::NONE, terminal)
{}

Input::Combo::Combo(Moveable terminal)
: Combo(Pressable::NONE, terminal)
{}

bool Input::Combo::operator==(const Combo& rhs) const
{
	return (this->_pressableTerminal == rhs._pressableTerminal) && (this->_moveableTerminal == rhs._moveableTerminal) && (this->_modifiers == rhs._modifiers);
}

bool Input::Combo::is_terminal_pressable() const
{
	return _pressableTerminal != Pressable::NONE;
}

Input::Pressable Input::Combo::get_pressable_terminal() const
{
	return _pressableTerminal;
}

Input::Moveable Input::Combo::get_moveable_terminal() const
{
	return _moveableTerminal;
}

const std::array<Input::Pressable, 3>&  Input::Combo::get_modifiers() const
{
	return _modifiers;
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