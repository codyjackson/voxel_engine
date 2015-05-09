#include "quad.h"

Quad::Quad(const Color& c, const glm::vec3& first, const glm::vec3& second, const glm::vec3& third, const glm::vec3& fourth, ClockWise vertexOrder)
	:_vertices{ { Vertex(first, c), Vertex(second, c), Vertex(third, c), Vertex(fourth, c) } }
{}

Quad::Quad(const Color& c, const glm::vec3& first, const glm::vec3& second, const glm::vec3& third, const glm::vec3& fourth, CounterClockWise vertexOrder)
	:_vertices{ { Vertex(fourth, c), Vertex(third, c), Vertex(second, c), Vertex(first, c) } }
{}

const std::array<Vertex, 4>& Quad::get_vertices() const
{
	return _vertices;
}