#include "player.h"

#include "browser/jsvalue.h"
#include "constants/vector.h"
#include "units.h"


namespace {
	float velocityMetersPerSecond = meters(10.0f);
}

Player::Player()
:_forwardVelocity(0.0f), _rightVelocity(0.0f)
{}

void Player::start_moving_forward()
{
	_forwardVelocity = velocityMetersPerSecond;
}

void Player::start_moving_backward()
{
	_forwardVelocity = -velocityMetersPerSecond;
}

void Player::stop_moving_forward_or_backward()
{
	_forwardVelocity = 0;
}

void Player::start_moving_left()
{
	_rightVelocity = -velocityMetersPerSecond;
}

void Player::start_moving_right()
{
	_rightVelocity = velocityMetersPerSecond;
}

void Player::stop_moving_left_or_right()
{
	_rightVelocity = 0;
}

void Player::rotate_yaw(float degrees)
{
	get_transform()->relative_orientation().rotate(Constants::Vec3::up, degrees);
}

void Player::rotate_pitch(float degrees)
{
	rotate_up(degrees);
}

void Player::tick(float timeStepInSeconds)
{
	move_forward(_forwardVelocity * timeStepInSeconds);
	move_right(_rightVelocity * timeStepInSeconds);
}

JSValue Player::create_ui_api()
{
	JSValue::Object root;
	root["startMovingForward"] = JSValue::wrap_void_function(std::bind(&Player::start_moving_forward, this));
	root["startMovingBackward"] = JSValue::wrap_void_function(std::bind(&Player::start_moving_backward, this));
	root["stopMovingForwardOrBackward"] = JSValue::wrap_void_function(std::bind(&Player::stop_moving_forward_or_backward, this));
	root["startMovingLeft"] = JSValue::wrap_void_function(std::bind(&Player::start_moving_left, this));
	root["startMovingRight"] = JSValue::wrap_void_function(std::bind(&Player::start_moving_right, this));
	root["stopMovingLeftOrRight"] = JSValue::wrap_void_function(std::bind(&Player::stop_moving_left_or_right, this));
	root["rotate"] = [this](const JSValue::Array& args){
		if ((args.size() != 2) || !args[0].is_double() || !args[1].is_double()) {
			throw std::runtime_error("Expected degrees for yaw and pitch as the two arguments.");
		}

		const float yaw = static_cast<float>(static_cast<double>(args[0]));
		const float pitch = static_cast<float>(static_cast<double>(args[1]));
		rotate_yaw(yaw);
		rotate_pitch(pitch);
		return JSValue::Array();
	};

	return root;
}