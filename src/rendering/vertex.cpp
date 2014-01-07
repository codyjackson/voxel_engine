#include "vertex.h"
#include <Windows.h>
#include <gl\GL.h>

Vertex::Vertex(const glm::vec3& p, const Color& c)
	:_position(p), _color(c)
{}

void Vertex::draw_with_color() const
{
	const float red = static_cast<float>(_color.get_blue())/255.0f;
	const float green = static_cast<float>(_color.get_blue())/255.0f;
	const float blue = static_cast<float>(_color.get_blue())/255.0f;

	glColor3f(red, green, blue);
	draw_without_color();
}

void Vertex::draw_without_color() const
{
	glVertex3f(_position.x, _position.y, _position.z);
}