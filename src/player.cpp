#include "player.h"

#include "constants\vector.h"
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
	get_transform()->orientation().rotate(Constants::Vec3::up, degrees);
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