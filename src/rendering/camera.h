#pragma once

#include <glm/glm.hpp>

#include "./../spatial/transform.h"

class Camera : public ITransformable
{
public:
	Camera();
	Camera(std::shared_ptr<Transform> transform);
	
	glm::mat4 get_view_matrix() const;
	glm::mat4 get_projection_matrix() const;
};