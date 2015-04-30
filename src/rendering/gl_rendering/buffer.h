#pragma once

#include "binder.h"

#include <vector>

typedef unsigned int Gluint;


class Buffer
{
public:
	enum class Type
	{
		ARRAY,
		COPY_READ,
		COPY_WRITE,
		ELEMENT_ARRAY,
		PIXEL_PACK,
		PIXEL_UNPACK,
		TEXTURE,
		TRANSFORM_FEEDBACK,
		UNIFORM
	};

	enum class Usage
	{
		STREAM_DRAW,
		STREAM_READ,
		STREAM_COPY,
		STATIC_DRAW,
		STATIC_READ,
		STATIC_COPY,
		DYNAMIC_DRAW,
		DYNAMIC_READ,
		DYNAMIC_COPY
	};

	Buffer(Type type, Usage usage);

	template <typename T>
	Buffer(Type type, Usage usage, const std::vector<T>& data)
		:Buffer(type, usage)
	{
		upload_data(data);
	}

	~Buffer();

	template<typename T>
	void upload_data(const std::vector<T>& data)
	{
		Binder<Buffer> binder(*this);
		glBufferDataForwarder(_type, sizeof(std::vector<T>::value_type) * data.size(), data.data(), _usage);
	}

	void bind() const;
	void unbind() const;

private:
	void glBufferDataForwarder(Type type, size_t size, const void* data, Usage usage);

	Gluint _id;
	Type _type;
	Usage _usage;
};