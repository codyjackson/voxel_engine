#pragma once

#include "../constants/vector.h"
#include "color.h"
#include "vertex.h"
#include <glm/glm.hpp>

#include <array>

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
		const glm::vec3 topLeft = translation;
		const glm::vec3 topRight = topLeft + Constants::Vec3::right;
		const glm::vec3 bottomRight = topRight + Constants::Vec3::down;
		const glm::vec3 bottomLeft = bottomRight + Constants::Vec3::left;
		return Quad(color, topLeft, topRight, bottomRight, bottomLeft, order);
	}

	template<typename T>
	static Quad generate_xz_quad(const Color& color, const glm::vec3& translation, T order)
	{
		const glm::vec3 frontLeft = translation;
		const glm::vec3 frontRight = frontLeft + Constants::Vec3::right;
		const glm::vec3 backRight = frontRight + Constants::Vec3::forward;
		const glm::vec3 backLeft = backRight + Constants::Vec3::left;
		return Quad(color, frontLeft, backLeft, backRight, frontRight, order);
	}

	template<typename T>
	static Quad generate_yz_quad(const Color& color, const glm::vec3& translation, T order)
	{
		const glm::vec3 frontTop = translation;
		const glm::vec3 frontBottom = frontTop + Constants::Vec3::down;
		const glm::vec3 backBottom = frontBottom + Constants::Vec3::forward;
		const glm::vec3 backTop = backBottom + Constants::Vec3::up;
		return Quad(color, frontTop, frontBottom, backBottom, backTop, order);
	}

	const std::array<Vertex, 4>& get_vertices() const;

private:
	std::array<Vertex, 4> _vertices;
};