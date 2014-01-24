#include "transform.h"
#include <glm\gtx\transform.hpp>

std::shared_ptr<Transform> Transform::make_transform()
{
	return std::make_shared<Transform>();
}
std::shared_ptr<Transform> Transform::make_transform(const glm::vec3& position, const Orientation& orientation)
{
	//I would rather use make shared here but unfortunately it seems that friend the function that creates the
	//object is not portable.
	return std::shared_ptr<Transform>(new Transform(position, orientation, std::shared_ptr<Transform>()));
}
std::shared_ptr<Transform> Transform::make_transform(const glm::vec3& position, const Orientation& orientation, std::shared_ptr<Transform> parent)
{
	//I would rather use make shared here but unfortunately it seems that friend the function that creates the
	//object is not portable.
	return std::shared_ptr<Transform>(new Transform(position, orientation, parent));
}

void Transform::kill_parent()
{
	_parent.reset();
}
void Transform::parent(std::shared_ptr<Transform> parent)
{
	_parent = parent;
}

glm::vec3& Transform::position()
{
	return _position;
}
Orientation& Transform::orientation()
{
	return _orientation;
}
const glm::vec3& Transform::position() const
{
	return _position;
}
const Orientation& Transform::orientation() const
{
	return _orientation;
}

glm::mat4 Transform::get_model_matrix() const
{
	if (_parent)
		return _parent->get_model_matrix() * glm::translate(_position) * _orientation.get_rotation_matrix();
	else
		return glm::translate(_position) * _orientation.get_rotation_matrix();

}

Transform::Transform()
{
}

Transform::Transform(const glm::vec3& position, const Orientation& orientation, std::shared_ptr<Transform> parent)
: _position(position), _orientation(orientation), _parent(parent)
{

}