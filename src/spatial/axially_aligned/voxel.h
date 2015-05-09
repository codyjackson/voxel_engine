#pragma once

#include "../../rendering/mesh.h"

#include "../intersection.h"
#include "../ray.h"

#include "plane.h"

#include <glm/glm.hpp>

namespace AxiallyAligned
{
	class Voxel
	{
	public:
		enum class Face { TOP, BOTTOM, FRONT, BACK, LEFT, RIGHT };

		class Intersected
		{
		public:
			Intersected(Face face);
			Face get_face() const;

		private:
			Face _face;
		};

		Voxel(const glm::vec3& topLeftFront, float sideLength);

		bool is_inside(const Ray& r) const;
		Intersection<Intersected> find_intersection(const Ray& r) const;

		Mesh::Builder generate_mesh_builder(const Color& c, bool isFrontVisible, bool isBackVisible, bool isTopVisible, bool isBottomVisible, bool isLeftVisible, bool isRightVisible) const;
		std::shared_ptr<Mesh> generate_mesh(const Color& c, bool isFrontVisible, bool isBackVisible, bool isTopVisible, bool isBottomVisible, bool isLeftVisible, bool isRightVisible) const;

	private:
		Intersection<Intersected> get_top_face_intersection(const Ray& r) const;
		Intersection<Intersected> get_bottom_face_intersection(const Ray& r) const;
		Intersection<Intersected> get_left_face_intersection(const Ray& r) const;
		Intersection<Intersected> get_right_face_intersection(const Ray& r) const;
		Intersection<Intersected> get_front_face_intersection(const Ray& r) const;
		Intersection<Intersected> get_back_face_intersection(const Ray& r) const;

		inline float top() const { return _topLeftFront.y; }
		inline float bottom() const{ return top() - _sideLength; }
		inline float left() const{ return _topLeftFront.x; }
		inline float right() const{ return left() + _sideLength; }
		inline float front() const{ return _topLeftFront.z; }
		inline float back() const{ return front() - _sideLength; }

		glm::vec3 _topLeftFront;
		float _sideLength;
	};
}