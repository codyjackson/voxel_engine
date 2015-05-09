#pragma once

#include "quad.h"

#include "gl_rendering/vertex_buffer_object.h"

#include <vector>

class Mesh : boost::noncopyable
{
public:
	class Builder
	{
	public:
		size_t size() const;

		Builder& push_back(const Quad& q);
		Builder& concatenate(const Builder& m);

		std::shared_ptr<Mesh> build() const;

	private:
		std::vector<Quad> _quads;
	};
protected:
	Mesh(const std::vector<Quad>& quads);

private:
	VertexBufferObject _vbo;
};