#pragma once

#include "buffer.h"
#include "vertex_array_object.h"

#include <boost/noncopyable.hpp>

#include <memory>

class VertexBufferObject : boost::noncopyable
{
public:
	template<typename T>
	VertexBufferObject(Buffer::Usage usage, const std::vector<T>& data, std::shared_ptr<const VertexArrayObject> vao)
		:_buffer(Buffer::Type::ARRAY, usage, data), _vao(vao)
	{
		if (!vao) {
			throw std::runtime_error("Vao was not initialized.");
		}
	}

	void bind() const;
	void unbind() const;

private:
	Buffer _buffer;
	std::shared_ptr<const VertexArrayObject> _vao;
};