#pragma once

#include "../color.h"

#include "../vertex.h"

#include <glm/glm.hpp>

#include <boost/noncopyable.hpp>

#include <exception>
#include <memory>
#include <vector>

typedef unsigned int GLuint;

class VertexArrayObject : boost::noncopyable
{
protected:
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

public:
	template<typename T>
	static std::shared_ptr<VertexArrayObject> make()
	{
		static_assert(!"The type specified is not yet supported.");
	}

	template<>
	static  std::shared_ptr<VertexArrayObject> make<Vertex>()
	{
		std::vector<Attribute> attributes;
		attributes.push_back(Attribute(3, Attribute::Type::FLOAT, 0));
		attributes.push_back(Attribute(4, Attribute::Type::FLOAT, 0));
		return make(std::move(attributes));
	}

	template<>
	static  std::shared_ptr<VertexArrayObject> make<glm::vec3>()
	{
		std::vector<Attribute> attributes;
		attributes.push_back(Attribute(3, Attribute::Type::FLOAT, 0));
		return make(std::move(attributes));
	}

	~VertexArrayObject();

	void bind() const;
	void unbind() const;
protected:
	VertexArrayObject(const std::vector<Attribute>&& attributes);

private:
	static std::shared_ptr<VertexArrayObject> make(const std::vector<Attribute>&& attributes);

	GLuint _id;
	std::vector<Attribute> _attributes;
};