#pragma once

#include <glm/glm.hpp>

#include "./../spatial/ray.h"
#include "./../spatial/transform.h"

class Camera : public ITransformable
{
public:
	Camera();
	Camera(std::shared_ptr<Transform> transform);
	
	Ray get_ray_projected_through_center() const;
	glm::mat4 get_view_matrix() const;
	glm::mat4 get_projection_matrix() const;
};