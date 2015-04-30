#include "buffer.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "error.h"

namespace {
	GLenum toGl(Buffer::Type type)
	{
		switch (type) {
			case Buffer::Type::ARRAY:
				return GL_ARRAY_BUFFER;
			case Buffer::Type::COPY_READ:
				return GL_COPY_READ_BUFFER;
			case Buffer::Type::COPY_WRITE:
				return GL_COPY_WRITE_BUFFER;
			case Buffer::Type::ELEMENT_ARRAY:
				return GL_ELEMENT_ARRAY_BUFFER;
			case Buffer::Type::PIXEL_PACK:
				return GL_PIXEL_PACK_BUFFER;
			case Buffer::Type::PIXEL_UNPACK:
				return GL_PIXEL_UNPACK_BUFFER;
			case Buffer::Type::TEXTURE:
				return GL_TEXTURE_BUFFER;
			case Buffer::Type::TRANSFORM_FEEDBACK:
				return GL_TRANSFORM_FEEDBACK_BUFFER;
			case Buffer::Type::UNIFORM:
				return GL_UNIFORM_BUFFER;
			default:
				throw std::runtime_error("Unsupported type.");
		}

	}

	GLenum toGl(Buffer::Usage usage)
	{
		switch (usage) {
			case Buffer::Usage::STREAM_DRAW:
				return GL_STREAM_DRAW;
			case Buffer::Usage::STREAM_READ:
				return GL_STREAM_READ;
			case Buffer::Usage::STREAM_COPY:
				return GL_STREAM_COPY;
			case Buffer::Usage::STATIC_DRAW:
				return GL_STATIC_DRAW;
			case Buffer::Usage::STATIC_READ:
				return GL_STATIC_READ;
			case Buffer::Usage::STATIC_COPY:
				return GL_STATIC_COPY;
			case Buffer::Usage::DYNAMIC_DRAW:
				return GL_DYNAMIC_DRAW;
			case Buffer::Usage::DYNAMIC_READ:
				return GL_DYNAMIC_READ;
			case Buffer::Usage::DYNAMIC_COPY:
				return GL_DYNAMIC_COPY;
			default:
				throw std::runtime_error("Unsupported usage.");
		}
	}
}

Buffer::Buffer(Type type, Usage usage)
:_type(type), _usage(usage)
{
	glGenBuffers(1, &_id); gl_error_check();
}

Buffer::~Buffer()
{
	if (_id) {
		glDeleteBuffers(1, &_id); gl_error_check();
	}
}

void Buffer::bind() const
{
	glBindBuffer(toGl(_type), _id); gl_error_check();
}

void Buffer::unbind() const
{
	glBindBuffer(toGl(_type), 0); gl_error_check();
}

void Buffer::glBufferDataForwarder(Type type, size_t size, const void* data, Usage usage)
{
	glBufferData(toGl(type), size, data, toGl(usage)); gl_error_check();
}