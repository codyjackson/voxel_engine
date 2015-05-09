#include "vertex_array_object.h"

#include "error.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <algorithm>

namespace
{
	struct VertexArrayObjectMaker : public VertexArrayObject
	{
		VertexArrayObjectMaker(const std::vector<VertexArrayObject::Attribute>&& attributes)
			:VertexArrayObject(std::move(attributes))
		{}
	};
}

std::shared_ptr<VertexArrayObject> VertexArrayObject::make(const std::vector<Attribute>&& attributes)
{
	return std::make_shared<VertexArrayObjectMaker>(std::move(attributes));
}

VertexArrayObject::VertexArrayObject(const std::vector<Attribute>&& attributes)
:_id(0), _attributes(attributes)
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
	glBindVertexArray(_id); gl_error_check();
	for (unsigned int i = 0; i < _attributes.size(); ++i) {
		const Attribute& attribute = _attributes[i];
		glEnableVertexAttribArray(i); gl_error_check();
		glVertexAttribPointer(i, attribute.size, attribute.type, GL_FALSE, 0, 0); gl_error_check();
	}
}

void VertexArrayObject::unbind() const
{
	for (unsigned int i = 0; i < _attributes.size(); ++i) {
		glDisableVertexAttribArray(i); gl_error_check();
	}
	glBindVertexArray(0);
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