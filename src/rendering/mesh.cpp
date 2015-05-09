#include "mesh.h"

#include <Windows.h>

#include <GLFW/glfw3.h>

#include <algorithm>
#include <functional>

namespace
{
	struct MeshMaker : public Mesh
	{
		MeshMaker(const std::vector<Quad>& quads)
			:Mesh(quads)
		{}
	};
}

Mesh::Builder& Mesh::Builder::push_back(const Quad& q)
{
	_quads.push_back(q);
	return *this;
}

Mesh::Builder& Mesh::Builder::concatenate(const Builder& rhs)
{
	_quads.insert(_quads.end(), rhs._quads.begin(), rhs._quads.end());
	return *this;
}

size_t Mesh::Builder::size() const
{
	return _quads.size();
}

std::shared_ptr<Mesh> Mesh::Builder::build() const
{
	return std::make_shared<MeshMaker>(_quads);
}

Mesh::Mesh(const std::vector<Quad>& quads)
:_vbo(Buffer::Usage::STATIC_DRAW, quads, VertexArrayObject::make<Vertex>())
{}