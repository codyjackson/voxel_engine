#include "voxel.h"

#include "../../constants/vector.h"

namespace AxiallyAligned
{
	Voxel::Voxel(const glm::vec3& topLeftFront, float sideLength)
		:_topLeftFront(topLeftFront), _sideLength(sideLength)
	{}

	Intersection<Voxel> Voxel::find_intersection(const Ray& r) const
	{
		if (const auto intersection = r.direction().x < 0 ? get_right_face_intersection(r) : get_left_face_intersection(r))
			return make_intersection<Voxel>(intersection->get_distance_from_origin(), *this);

		if (const auto intersection = r.direction().y < 0 ? get_top_face_intersection(r) : get_bottom_face_intersection(r))
			return make_intersection<Voxel>(intersection->get_distance_from_origin(), *this);

		if (const auto intersection = r.direction().z < 0 ? get_front_face_intersection(r) : get_back_face_intersection(r))
			return make_intersection<Voxel>(intersection->get_distance_from_origin(), *this);

		return Intersection<Voxel>();
	}
	

	Intersection<Plane> Voxel::get_top_face_intersection(const Ray& r) const
	{
		const Plane p(Plane::Span::XZ, top());
		const auto intersection = p.find_intersection(r);
		if (!intersection)
			return false;

		const auto pointOfIntersection = r.cast(intersection->get_distance_from_origin());
		if (is_between(pointOfIntersection.x, left(), right()) && is_between(pointOfIntersection.z, front(), back()))
			return make_intersection<Plane>(intersection->get_distance_from_origin(), p);

		return Intersection<Plane>();
	}

	Intersection<Plane> Voxel::get_bottom_face_intersection(const Ray& r) const
	{
		const Plane p(Plane::Span::XZ, bottom());
		const auto intersection = p.find_intersection(r);
		if (!intersection)
			return false;

		const auto pointOfIntersection = r.cast(intersection->get_distance_from_origin());
		if (is_between(pointOfIntersection.x, left(), right()) && is_between(pointOfIntersection.z, front(), back()))
			return make_intersection<Plane>(intersection->get_distance_from_origin(), p);

		return Intersection<Plane>();
	}

	Intersection<Plane> Voxel::get_left_face_intersection(const Ray& r) const
	{
		const Plane p(Plane::Span::YZ, left());
		const auto intersection = p.find_intersection(r);
		if (!intersection)
			return false;

		const auto pointOfIntersection = r.cast(intersection->get_distance_from_origin());
		if (is_between(pointOfIntersection.y, top(), bottom()) && is_between(pointOfIntersection.z, front(), back()))
			return make_intersection<Plane>(intersection->get_distance_from_origin(), p);

		return Intersection<Plane>();
	}

	Intersection<Plane> Voxel::get_right_face_intersection(const Ray& r) const
	{
		const Plane p(Plane::Span::YZ, right());
		const auto intersection = p.find_intersection(r);
		if (!intersection)
			return false;

		const auto pointOfIntersection = r.cast(intersection->get_distance_from_origin());
		if (is_between(pointOfIntersection.y, top(), bottom()) && is_between(pointOfIntersection.z, front(), back()))
			return make_intersection<Plane>(intersection->get_distance_from_origin(), p);

		return Intersection<Plane>();
	}

	Intersection<Plane> Voxel::get_front_face_intersection(const Ray& r) const
	{
		const Plane p(Plane::Span::XY, front());
		const auto intersection = p.find_intersection(r);
		if (!intersection)
			return false;

		const auto pointOfIntersection = r.cast(intersection->get_distance_from_origin());
		if (is_between(pointOfIntersection.x, left(), right()) && is_between(pointOfIntersection.y, top(), bottom()))
			return make_intersection<Plane>(intersection->get_distance_from_origin(), p);

		return Intersection<Plane>();
	}

	Intersection<Plane> Voxel::get_back_face_intersection(const Ray& r) const
	{
		const Plane p(Plane::Span::XY, back());
		const auto intersection = p.find_intersection(r);
		if (!intersection)
			return false;

		const auto pointOfIntersection = r.cast(intersection->get_distance_from_origin());
		if (is_between(pointOfIntersection.x, left(), right()) && is_between(pointOfIntersection.y, top(), bottom()))
			return make_intersection<Plane>(intersection->get_distance_from_origin(), p);

		return Intersection<Plane>();
	}
}