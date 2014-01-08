#include "frame_of_referece.h"
#include <glm/gtx/rotate_vector.hpp>

FrameOfReference::FrameOfReference(const glm::vec3& position, const glm::vec3& up, const glm::vec3& forward)
:FrameOfReference(position, up, forward, 1.0f)
{}

FrameOfReference::FrameOfReference(const glm::vec3& position, const glm::vec3& up, const glm::vec3& forward, float scale)
: _position(position), _up(up), _forward(forward), _scale(scale)
{}

void FrameOfReference::translate(const glm::vec3& translation)
{
	_position += translation;
}

void FrameOfReference::rotate(const glm::vec3& axisOfRotation, float degreesOfRotation)
{
	_up = glm::rotate(_up, degreesOfRotation, axisOfRotation);
	_forward = glm::rotate(_forward, degreesOfRotation, axisOfRotation);
}

void FrameOfReference::scale(float scale)
{
	_scale = scale;
}

glm::vec3 FrameOfReference::get_position() const
{
	return _position;
}

glm::vec3 FrameOfReference::get_forward() const
{
	return _forward;
}

glm::vec3 FrameOfReference::get_up() const
{
	return _up;
}

float FrameOfReference::get_scale() const
{
	return _scale;
}

glm::mat4 FrameOfReference::get_model_matrix() const
{
}