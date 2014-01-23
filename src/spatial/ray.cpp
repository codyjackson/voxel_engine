#include "ray.h"

Ray::Ray(const glm::vec3& origin, const glm::vec3& direction)
:_origin(origin), _direction(direction)
{}

glm::vec3 Ray::origin() const
{
	return _origin;
}

glm::vec3 Ray::direction() const
{
	return _direction;
}

glm::vec3 Ray::cast(float distance) const
{
	return _origin + (distance * _direction);
}