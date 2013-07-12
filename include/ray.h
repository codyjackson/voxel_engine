#pragma once

#include <glm/glm.hpp>

class Ray
{
	public:
		Ray();
		Ray(const glm::vec3 origin, const glm::vec3 dir);
	private:
		glm::vec3 origin;
		glm::vec3 direction;
}