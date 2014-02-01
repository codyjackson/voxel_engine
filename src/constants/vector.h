#pragma once

#include <glm/glm.hpp>
namespace Constants
{
	struct Vec3
	{
		static const glm::vec3 up;
		static const glm::vec3 down;
		static const glm::vec3 left;
		static const glm::vec3 right;
		static const glm::vec3 forward;
		static const glm::vec3 backward;
	};

	struct IVec3
	{
		static const glm::ivec3 up;
		static const glm::ivec3 down;
		static const glm::ivec3 left;
		static const glm::ivec3 right;
		static const glm::ivec3 forward;
		static const glm::ivec3 backward;
	};

	struct Vec4
	{
		static const glm::vec4 up;
		static const glm::vec4 down;
		static const glm::vec4 left;
		static const glm::vec4 right;
		static const glm::vec4 forward;
		static const glm::vec4 backward;
	};
}