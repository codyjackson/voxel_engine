#include "vertex_buffer_object.h"
#include "binder.h"


void VertexBufferObject::bind() const
{
	_buffer.bind();
	_vao->bind();

}

void VertexBufferObject::unbind() const
{
	_vao->unbind();
	_buffer.unbind();
}


