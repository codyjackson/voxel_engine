#pragma once

#include "orientation.h"

#include <memory>

class Transform
{
public:
	Transform();
	Transform(const glm::vec3& position, const Orientation& orientation, std::shared_ptr<Transform> parent);

	static std::shared_ptr<Transform> make_transform();
	static std::shared_ptr<Transform> make_transform(const glm::vec3& position, const Orientation& orientation);
	static std::shared_ptr<Transform> make_transform(const glm::vec3& position, const Orientation& orientation, std::shared_ptr<Transform> parent);

	void kill_parent();
	void parent(std::shared_ptr<Transform> parent);

	glm::vec3& position();
	Orientation& orientation();
	const glm::vec3& position() const;
	const Orientation& orientation() const;

	glm::mat4 get_model_matrix() const;

private:
	friend std::shared_ptr<Transform> std::make_shared<>();

	

	Transform(const Transform& other); //Don't need to define. I'm making this class non copyable. ex: boost::noncopyable
	Transform& operator=(const Transform&); //Don't need to define.

	glm::vec3 _position;
	Orientation _orientation;
	std::shared_ptr<Transform> _parent;
};