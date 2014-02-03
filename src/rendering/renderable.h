#pragma once

#include "mesh.h"

#include <glm/glm.hpp>

class Renderable
{
public:
	Renderable(const Mesh& mesh, const glm::mat4& modelMatrix);

	const Mesh& get_mesh() const;
	const glm::mat4& get_model_matrix() const;

private:
	const Mesh& _mesh;
	const glm::mat4& _modelMatrix;
};