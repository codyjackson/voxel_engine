#include "Vector.h"

namespace Constants
{
	const glm::vec3 Vec3::up = glm::vec3(0, 1, 0);
	const glm::vec3 Vec3::down = glm::vec3(0, -1, 0);
	const glm::vec3 Vec3::left = glm::vec3(-1, 0, 0);
	const glm::vec3 Vec3::right = glm::vec3(1, 0, 0);
	const glm::vec3 Vec3::forward = glm::vec3(0, 0, -1);
	const glm::vec3 Vec3::backward = glm::vec3(0, 0, 1);

	const glm::vec4 Vec4::up = glm::vec4(0, 1, 0, 0);
	const glm::vec4 Vec4::down = glm::vec4(0, -1, 0, 0);
	const glm::vec4 Vec4::left = glm::vec4(-1, 0, 0, 0);
	const glm::vec4 Vec4::right = glm::vec4(1, 0, 0, 0);
	const glm::vec4 Vec4::forward = glm::vec4(0, 0, -1, 0);
	const glm::vec4 Vec4::backward = glm::vec4(0, 0, 1, 0);
}