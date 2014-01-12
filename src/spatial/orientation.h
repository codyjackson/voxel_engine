#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Orientation
{
public:
	Orientation();

	void rotate(const glm::vec3& axis, const float degreesOfRotation);

	glm::mat4 get_rotation_matrix() const;

	glm::vec3 forward() const;
	glm::vec3 backward() const;
	glm::vec3 up() const;
	glm::vec3 down() const;
	glm::vec3 right() const;
	glm::vec3 left() const;

private:
	glm::quat _quaternion;
	glm::vec4 _forward;
	glm::vec4 _up;
	glm::vec4 _right;
};