#include "camera.h"

#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(const glm::vec3& position, const Orientation& orientation)
:ITransformable(Transform::make_transform(position, orientation))
{}

glm::mat4 Camera::get_view_matrix() const
{
	return glm::inverse(_transform->get_model_matrix());
}

glm::mat4 Camera::get_projection_matrix() const
{
	return glm::perspective(90.0f, 1024.0f/768.0f, 0.00001f, 1000.0f);
}