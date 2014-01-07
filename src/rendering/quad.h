#pragma once

#include "color.h"
#include "vertex.h"
#include <glm/glm.hpp>

class Quad
{
public:
	struct ClockWise{};
	struct CounterClockWise{};

	Quad(const Color& c, const glm::vec3& first, const glm::vec3& second, const glm::vec3& third, const glm::vec3& fourth, ClockWise vertexOrder);
	Quad(const Color& c, const glm::vec3& first, const glm::vec3& second, const glm::vec3& third, const glm::vec3& fourth, CounterClockWise vertexOrder);

	void draw_with_color() const;
	void draw_without_color() const;

private:
	//Drawn in order from 1st to 4th.
	Vertex _first;
	Vertex _second;
	Vertex _third;
	Vertex _fourth;
};