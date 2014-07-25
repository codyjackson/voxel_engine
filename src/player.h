#pragma once

class Player
{
public:
	void start_moving_forward();
	void stop_moving_forward();

	void start_moving_backward();
	void stop_moving_backward();

	void start_moving_left();
	void stop_moving_left();

	void start_moving_right();
	void stop_moving_right();

	void rotate_yaw(float degrees);
	void rotate_pitch(float degrees);

private:
};