#include "vertex.h"
#include <Windows.h>
#include <gl\GL.h>

Vertex::Vertex(const glm::vec3& p, const Color& c)
	:_position(p), _color(c)
{}

void Vertex::draw_with_color() const
{
	glColor3ub(_color.get_red(), _color.get_green(), _color.get_blue());
	draw_without_color();
}

void Vertex::draw_without_color() const
{
	glVertex3f(_position.x, _position.y, _position.z);
}