#include "renderable.h"

Renderable::Renderable(const Mesh& mesh, const glm::mat4& modelMatrix)
:_mesh(mesh), _modelMatrix(modelMatrix)
{}

const Mesh& Renderable::get_mesh() const
{
	return _mesh;
}

const glm::mat4& Renderable::get_model_matrix() const
{
	return _modelMatrix;
}