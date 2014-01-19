#include "camera.h"

#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(const glm::vec3& position, const Orientation& orientation)
:position(position), orientation(orientation)
{}

glm::mat4 Camera::get_view_matrix() const
{
	const glm::mat4 rotation = orientation.get_rotation_matrix();
	const glm::mat4 translation = glm::translate(position);
	return glm::inverse(translation * rotation);
}

glm::mat4 Camera::get_projection_matrix() const
{
	return glm::perspective(90.0f, 1024.0f/768.0f, 0.1f, 1000.0f);
}