#pragma once

#include <glm/glm.hpp>

#include "./../spatial/orientation.h"

class Camera
{
public:
	Camera(const glm::vec3& position, const Orientation& orientation);
	
	glm::mat4 get_view_matrix() const;
	glm::mat4 get_projection_matrix() const;

	glm::vec3 position;
	Orientation orientation;
};