#include "camera.h"

#include <glm/gtx/transform.hpp>

Camera::Camera(const glm::vec3& position, const Orientation& orientation)
:position(position), orientation(orientation)
{}

glm::mat4 Camera::get_view_matrix() const
{
	return glm::lookAt(position, orientation.forward(), orientation.up());
}

glm::mat4 Camera::get_projection_matrix() const
{
	return glm::mat4();
}