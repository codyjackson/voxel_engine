#pragma once

#include <glm/glm.hpp>

class Ray
{
public:
	Ray(const glm::vec3& origin, const glm::vec3& direction);

	glm::vec3 origin() const;
	glm::vec3 direction() const;

	glm::vec3 cast(float distance) const;



private:
	glm::vec3 _origin;
	glm::vec3 _direction;
};