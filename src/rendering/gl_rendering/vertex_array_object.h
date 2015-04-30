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
	void addVertexAttribute<glm::vec3>()
	{
		_attributes.push_back(Attribute(3, Attribute::Type::FLOAT, 0));
	}

	template<>
	void addVertexAttribute<glm::vec4>()
	{
		_attributes.push_back(Attribute(4, Attribute::Type::FLOAT, 0));
	}

	template<>
	void addVertexAttribute<Color>()
	{
		_attributes.push_back(Attribute(4, Attribute::Type::FLOAT, 0));
	}

	void bind() const;
	void unbind() const;

private:
	struct Attribute
	{
		enum class Type
		{
			FLOAT
		};

		Attribute(GLuint size, Type type, GLuint stride);
		const GLuint size;
		const GLuint type;
		const GLuint stride;

	private: 
		static GLuint toGl(Type type);
	};

	GLuint _id;
	std::vector<Attribute> _attributes;
};