#pragma once

#include "orientation.h"

#include <boost/noncopyable.hpp>
#include <memory>

class Transform : boost::noncopyable
{
public:
	static std::shared_ptr<Transform> make_transform();
	static std::shared_ptr<Transform> make_transform(std::shared_ptr<Transform> rhs);
	static std::shared_ptr<Transform> make_transform(const glm::vec3& position, const Orientation& orientation);
	static std::shared_ptr<Transform> make_transform(const glm::vec3& position, const Orientation& orientation, std::shared_ptr<const Transform> parent);

	glm::vec3& position();
	const glm::vec3& position() const;

	Orientation& orientation();
	const Orientation& orientation() const;

	glm::mat4 get_model_matrix() const;


protected:
	Transform();
	Transform(const glm::vec3& position, const Orientation& orientation);
	Transform(const glm::vec3& position, const Orientation& orientation, std::shared_ptr<const Transform> parent);

private:
	glm::vec3 _position;
	Orientation _orientation;
	std::shared_ptr<const Transform> _parent;
};

class ITransformable
{
public:
	ITransformable();
	ITransformable(std::shared_ptr<Transform> transform);

	std::shared_ptr<Transform> get_transform();
	const std::shared_ptr<const Transform> get_transform() const;

protected:
	std::shared_ptr<Transform> _transform;
};