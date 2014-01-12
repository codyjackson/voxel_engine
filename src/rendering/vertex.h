#pragma once

#include "color.h"
#include "vertex.h"
#include <glm\glm.hpp>

class Vertex
{
public:
	Vertex(const glm::vec4& p, const Color& c);
	Vertex(const glm::vec3& p, const Color& c);

	void draw_with_color() const;
	void draw_without_color() const;


private:
	glm::vec4 _position;
	Color _color;
};