#include "vertex_array_object.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

VertexArrayObject::VertexArrayObject()
:_id(0)
{
	glGenVertexArrays(1, &_id);
}

VertexArrayObject::~VertexArrayObject()
{
	if (_id) {
		glDeleteVertexArrays(1, &_id);
	}
}

template<>
void VertexArrayObject::addVertexAttribute<glm::vec3>()
{
	_attributes.push_back(Attribute(3, GL_FLOAT, 0));
}

template<>
void VertexArrayObject::addVertexAttribute<glm::vec4>()
{
	_attributes.push_back(Attribute(4, GL_FLOAT, 0));
}

template<>
void VertexArrayObject::addVertexAttribute<Color>()
{
	_attributes.push_back(Attribute(4, GL_FLOAT, 0));
}

VertexArrayObject::Attribute::Attribute(GLuint size, GLuint type, GLuint stride)
:size(size), type(type), stride(stride)
{}