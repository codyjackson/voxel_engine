#pragma once

#include <glm/glm.hpp>

class Ray
{
public:
	Ray(const glm::vec3& origin, const glm::vec3& direction);

	glm::vec3 origin() const;
	glm::vec3 direction() const;

	Ray transform_into_new_space(const glm::mat4& transformation) const;

	glm::vec3 cast(float distance) const;



private:
	glm::vec4 _origin;
	glm::vec4 _direction;
};