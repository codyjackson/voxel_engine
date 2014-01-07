#include "quad.h"

Quad::Quad(const Color& c, const glm::vec3& first, const glm::vec3& second, const glm::vec3& third, const glm::vec3& fourth, ClockWise vertexOrder)
	:_first(first, c), _second(second, c), _third(third, c), _fourth(fourth, c)
{}

Quad::Quad(const Color& c, const glm::vec3& first, const glm::vec3& second, const glm::vec3& third, const glm::vec3& fourth, CounterClockWise vertexOrder)
	:_first(fourth, c), _second(third, c), _third(second, c), _fourth(first, c)
{}

void Quad::draw_with_color() const
{
	_first.draw_with_color();
	_second.draw_without_color();
	_third.draw_without_color();
	_fourth.draw_without_color();
}

void Quad::draw_without_color() const
{
	_first.draw_without_color();
	_second.draw_without_color();
	_third.draw_without_color();
	_fourth.draw_without_color();
}