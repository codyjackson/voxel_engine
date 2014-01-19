#include "orientation.h"
#include "./../constants/vector.h"

Orientation::Orientation()
:_forward(Constants::Vec4::forward), _right(Constants::Vec4::right), _up(Constants::Vec4::up)
{}

void Orientation::rotate(const glm::vec3& axis, const float degreesOfRotation)
{
	_quaternion = glm::rotate(_quaternion, degreesOfRotation, axis);
	_quaternion = glm::normalize(_quaternion);
	const glm::mat4 rotation = get_rotation_matrix();
	_forward = rotation * Constants::Vec4::forward;
	_right = rotation * Constants::Vec4::right;
	_up = rotation * Constants::Vec4::up;
}

glm::mat4 Orientation::get_rotation_matrix() const
{
	return glm::mat4_cast(glm::inverse(_quaternion));
}

glm::vec3 Orientation::forward() const
{
	return glm::vec3(_forward);
}

glm::vec3 Orientation::backward() const
{
	return -1.0f * forward();
}

glm::vec3 Orientation::up() const
{
	return glm::vec3(_up);
}

glm::vec3 Orientation::down() const
{
	return -1.0f * up();
}

glm::vec3 Orientation::right() const
{
	return glm::vec3(_right);
}

glm::vec3 Orientation::left() const
{
	return -1.0f * right();
}