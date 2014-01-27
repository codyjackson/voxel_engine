#include "ray.h"

Ray::Ray(const glm::vec3& origin, const glm::vec3& direction)
:_origin(origin.x, origin.y, origin.z, 1.0f), _direction(direction.x, direction.y, direction.z, 0.0f)
{}

glm::vec3 Ray::origin() const
{
	return glm::vec3(_origin);
}

glm::vec3 Ray::direction() const
{
	return glm::vec3(_direction);
}

Ray Ray::transform_into_new_space(const glm::mat4& transformation) const
{
	const glm::vec3 origin = glm::vec3(transformation * _origin);
	const glm::vec3 direction = glm::vec3(transformation * _direction);

	return Ray(origin, direction);
}

glm::vec3 Ray::cast(float distance) const
{
	return glm::vec3(_origin) + (distance * glm::vec3(_direction));
}