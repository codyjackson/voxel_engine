#include "transform.h"
#include <glm\gtx\transform.hpp>

namespace
{
	//This class is used in order to give std::make_shared to give std::make_shared 
	//"access" to the Transform private constructors.
	class InstantiableTransform : public Transform
	{
	public:
		template<typename... Args>
		InstantiableTransform(Args&&... args)
			:Transform(std::forward<Args>(args)...)
		{}
	};
}

std::shared_ptr<Transform> Transform::make_transform()
{
	return std::make_shared<InstantiableTransform>();
}

std::shared_ptr<Transform> Transform::make_transform(std::shared_ptr<Transform> rhs)
{
	assert(rhs);
	return rhs ? std::make_shared<InstantiableTransform>(rhs->_position, rhs->_orientation, rhs->_parent) : std::shared_ptr<Transform>();
}

std::shared_ptr<Transform> Transform::make_transform(const glm::vec3& position, const Orientation& orientation)
{
	return std::make_shared<InstantiableTransform>(position, orientation);
}

std::shared_ptr<Transform> Transform::make_transform(const glm::vec3& position, const Orientation& orientation, std::shared_ptr<const Transform> parent)
{
	return std::make_shared<InstantiableTransform>(position, orientation, parent);
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
	auto localMatrix = glm::translate(_position) * _orientation.get_rotation_matrix();
	if (!_parent) {
		return localMatrix;
	}
	return _parent->get_model_matrix() * localMatrix;
}

Transform::Transform()
:_orientation()
{}

Transform::Transform(const glm::vec3& position, const Orientation& orientation)
: _position(position), _orientation(orientation)
{}

Transform::Transform(const glm::vec3& position, const Orientation& orientation, std::shared_ptr<const Transform> parent)
: _position(position), _orientation(orientation), _parent(parent)
{}

ITransformable::ITransformable()
: _transform(Transform::make_transform())
{}

ITransformable::ITransformable(std::shared_ptr<Transform> transform)
: _transform(transform)
{}

void ITransformable::move_forward(float distance)
{
	_transform->position() += _transform->orientation().forward() * distance;
}

void ITransformable::move_right(float distance)
{
	_transform->position() += _transform->orientation().right() * distance;
}

void ITransformable::move_up(float distance)
{
	_transform->position() += _transform->orientation().up() * distance;
}

void ITransformable::rotate_right(float degrees)
{
	_transform->orientation().rotate(_transform->orientation().up(), degrees);
}

void ITransformable::rotate_up(float degrees)
{
	_transform->orientation().rotate(_transform->orientation().right(), degrees);
}


std::shared_ptr<Transform> ITransformable::get_transform()
{
	return _transform;
}

const std::shared_ptr<const Transform> ITransformable::get_transform() const
{
	return std::static_pointer_cast<const Transform>(_transform);
}