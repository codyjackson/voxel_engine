#include "vertex_array_object.h"

#include "error.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <algorithm>

VertexArrayObject::VertexArrayObject()
:_id(0)
{
	glGenVertexArrays(1, &_id); gl_error_check();
}

VertexArrayObject::~VertexArrayObject()
{
	if (_id) {
		glDeleteVertexArrays(1, &_id); gl_error_check();
	}
}

void VertexArrayObject::bind() const
{
	for (unsigned int i = 0; i < _attributes.size(); ++i) {
		const Attribute& attribute = _attributes[i];
		glVertexAttribPointer(i, attribute.size, attribute.type, GL_FALSE, 0, 0); gl_error_check();
	}
}

void VertexArrayObject::unbind() const
{
	for (unsigned int i = 0; i < _attributes.size(); ++i) {
		glDisableVertexAttribArray(i); gl_error_check();
	}
}

VertexArrayObject::Attribute::Attribute(GLuint size, Type type, GLuint stride)
:size(size), type(toGl(type)), stride(stride)
{}

GLuint VertexArrayObject::Attribute::toGl(VertexArrayObject::Attribute::Type type)
{
	switch (type) {
	case Type::FLOAT:
		return GL_FLOAT;
	default:
		throw std::runtime_error("Invalid type.");
	}
}