#pragma once

#include "spatial/transform.h"
#include "browser/browser.h"

class Player : public ITransformable, public Browser::IJSObjectifiable
{
public:
	Player();

	void start_moving_forward();
	void start_moving_backward();
	void stop_moving_forward_or_backward();

	void start_moving_left();
	void start_moving_right();
	void stop_moving_left_or_right();

	void rotate_yaw(float degrees);
	void rotate_pitch(float degrees);

	void tick(float timeStepSeconds);

	JSValue create_js_object() override;

private:
	float _forwardVelocity;
	float _rightVelocity;

	std::shared_ptr<Transform> _transform;
};