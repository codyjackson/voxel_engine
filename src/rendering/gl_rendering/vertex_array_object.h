#pragma once

#include "../color.h"

#include <glm/glm.hpp>

#include <exception>
#include <vector>

typedef unsigned int GLuint;

class VertexArrayObject
{
public:
	VertexArrayObject();
	~VertexArrayObject();

	template<typename T>
	void addVertexAttribute()
	{
		static_assert(!"Unsupported attribute type");
	}

	template<>
	void addVertexAttribute<glm::vec3>();

	template<>
	void addVertexAttribute<glm::vec4>();

	template<>
	void addVertexAttribute<Color>();

private:
	struct Attribute
	{
		Attribute(GLuint size, GLuint type, GLuint stride);
		const GLuint size;
		const GLuint type;
		const GLuint stride;
	};

	GLuint _id;
	std::vector<Attribute> _attributes;
};