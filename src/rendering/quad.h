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

	template<typename T>
	static Quad generate_xy_quad(const Color& color, const glm::vec3& translation, T order)
	{
		glm::vec3 topLeft = glm::vec3(0, 0, 0) + translation;
		glm::vec3 topRight = glm::vec3(1, 0, 0) + translation;
		glm::vec3 bottomRight = glm::vec3(1, 1, 0) + translation;
		glm::vec3 bottomLeft = glm::vec3(0, 1, 0) + translation;
		return Quad(color, topLeft, topRight, bottomRight, bottomLeft, order);
	}

	template<typename T>
	static Quad generate_xz_quad(const Color& color, const glm::vec3& translation, T order)
	{
		glm::vec3 frontLeft = glm::vec3(0, 0, 0) + translation;
		glm::vec3 frontRight = glm::vec3(1, 0, 0) + translation;
		glm::vec3 backRight = glm::vec3(1, 0, 1) + translation;
		glm::vec3 backLeft = glm::vec3(0, 0, 1) + translation;
		return Quad(color, frontLeft, backLeft, backRight, frontRight, order);
	}

	template<typename T>
	static Quad generate_yz_quad(const Color& color, const glm::vec3& translation, T order)
	{
		glm::vec3 frontTop = glm::vec3(0, 0, 0) + translation;
		glm::vec3 frontBottom = glm::vec3(0, 1, 0) + translation;
		glm::vec3 backBottom = glm::vec3(0, 1, 1) + translation;
		glm::vec3 backRight = glm::vec3(0, 0, 1) + translation;
		return Quad(color, frontTop, frontBottom, backBottom, backRight, order);
	}

	void draw_with_color() const;
	void draw_without_color() const;

private:
	//Drawn in order from 1st to 4th.
	Vertex _first;
	Vertex _second;
	Vertex _third;
	Vertex _fourth;
};