#pragma once

#include "../intersection.h"
#include "../ray.h"
#include "plane.h"

#include <glm/glm.hpp>

namespace AxiallyAligned
{
	class Voxel
	{
	public:
		Voxel(const glm::vec3& topLeftFront, float sideLength);

		Intersection<Voxel> find_intersection(const Ray& r) const;

	private:
		static inline bool is_between(float x, float lower, float upper)
		{
			if (lower > upper)
				std::swap(lower, upper);
			return (x >= lower) && (x <= upper);
		}

		Intersection<Plane> get_top_face_intersection(const Ray& r) const;
		Intersection<Plane> get_bottom_face_intersection(const Ray& r) const;
		Intersection<Plane> get_left_face_intersection(const Ray& r) const;
		Intersection<Plane> get_right_face_intersection(const Ray& r) const;
		Intersection<Plane> get_front_face_intersection(const Ray& r) const;
		Intersection<Plane> get_back_face_intersection(const Ray& r) const;

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