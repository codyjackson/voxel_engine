#pragma once

#include <glm/glm.hpp>

class FrameOfReference
{
public:
	FrameOfReference(const glm::vec3& position, const glm::vec3& up, const glm::vec3& forward);
	FrameOfReference(const glm::vec3& position, const glm::vec3& up, const glm::vec3& forward, float scale);

	void translate(const glm::vec3& translation);
	void rotate(const glm::vec3& axisOfRotation, float degreesOfRotation);
	void scale(float scale);

	glm::vec3 get_position() const;
	glm::vec3 get_forward() const;
	glm::vec3 get_up() const;
	float get_scale() const;

	glm::mat4 get_model_matrix() const;

private:
	glm::vec3 _position;
	glm::vec3 _up;
	glm::vec3 _forward;
	float _scale;
};