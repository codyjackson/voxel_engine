#include "voxel.h"

#include "../../constants/vector.h"
#include "../../utility/numerical.h"

namespace AxiallyAligned
{
	Voxel::Voxel(const glm::vec3& topLeftFront, float sideLength)
		:_topLeftFront(topLeftFront), _sideLength(sideLength)
	{}

	Intersection<Voxel::Intersected> Voxel::find_intersection(const Ray& r) const
	{
		if (const auto intersection = r.direction().x < 0 ? get_right_face_intersection(r) : get_left_face_intersection(r))
			return intersection;

		if (const auto intersection = r.direction().y < 0 ? get_top_face_intersection(r) : get_bottom_face_intersection(r))
			return intersection;

		if (const auto intersection = r.direction().z < 0 ? get_front_face_intersection(r) : get_back_face_intersection(r))
			return intersection;

		return make_intersection<Voxel::Intersected>();
	}

	bool Voxel::is_inside(const Ray& r) const
	{
		return Numerical::is_between(r.origin().x, left(), right()) && Numerical::is_between(r.origin().y, top(), bottom()) && Numerical::is_between(r.origin().z, front(), back());
	}
	
	Intersection<Voxel::Intersected> Voxel::get_top_face_intersection(const Ray& r) const
	{
		const Plane p(Plane::Span::XZ, top());
		const auto intersection = p.find_intersection(r);
		if (!intersection)
			return false;

		const auto pointOfIntersection = r.cast(intersection->get_distance_from_origin());
		if (Numerical::is_between(pointOfIntersection.x, left(), right()) && Numerical::is_between(pointOfIntersection.z, front(), back()))
			return make_intersection<Voxel::Intersected>(intersection->get_distance_from_origin(), Face::TOP);

		return make_intersection<Voxel::Intersected>();
	}

	Intersection<Voxel::Intersected> Voxel::get_bottom_face_intersection(const Ray& r) const
	{
		const Plane p(Plane::Span::XZ, bottom());
		const auto intersection = p.find_intersection(r);
		if (!intersection)
			return false;

		const auto pointOfIntersection = r.cast(intersection->get_distance_from_origin());
		if (Numerical::is_between(pointOfIntersection.x, left(), right()) && Numerical::is_between(pointOfIntersection.z, front(), back()))
			return make_intersection<Voxel::Intersected>(intersection->get_distance_from_origin(), Face::BOTTOM);

		return make_intersection<Voxel::Intersected>();
	}

	Intersection<Voxel::Intersected> Voxel::get_left_face_intersection(const Ray& r) const
	{
		const Plane p(Plane::Span::YZ, left());
		const auto intersection = p.find_intersection(r);
		if (!intersection)
			return false;

		const auto pointOfIntersection = r.cast(intersection->get_distance_from_origin());
		if (Numerical::is_between(pointOfIntersection.y, top(), bottom()) && Numerical::is_between(pointOfIntersection.z, front(), back()))
			return make_intersection<Voxel::Intersected>(intersection->get_distance_from_origin(), Face::LEFT);

		return make_intersection<Voxel::Intersected>();
	}

	Intersection<Voxel::Intersected> Voxel::get_right_face_intersection(const Ray& r) const
	{
		const Plane p(Plane::Span::YZ, right());
		const auto intersection = p.find_intersection(r);
		if (!intersection)
			return false;

		const auto pointOfIntersection = r.cast(intersection->get_distance_from_origin());
		if (Numerical::is_between(pointOfIntersection.y, top(), bottom()) && Numerical::is_between(pointOfIntersection.z, front(), back()))
			return make_intersection<Voxel::Intersected>(intersection->get_distance_from_origin(), Face::RIGHT);

		return make_intersection<Voxel::Intersected>();
	}

	Intersection<Voxel::Intersected> Voxel::get_front_face_intersection(const Ray& r) const
	{
		const Plane p(Plane::Span::XY, front());
		const auto intersection = p.find_intersection(r);
		if (!intersection)
			return false;

		const auto pointOfIntersection = r.cast(intersection->get_distance_from_origin());
		if (Numerical::is_between(pointOfIntersection.x, left(), right()) && Numerical::is_between(pointOfIntersection.y, top(), bottom()))
			return make_intersection<Voxel::Intersected>(intersection->get_distance_from_origin(), Face::FRONT);

		return make_intersection<Voxel::Intersected>();
	}

	Intersection<Voxel::Intersected> Voxel::get_back_face_intersection(const Ray& r) const
	{
		const Plane p(Plane::Span::XY, back());
		const auto intersection = p.find_intersection(r);
		if (!intersection)
			return false;

		const auto pointOfIntersection = r.cast(intersection->get_distance_from_origin());
		if (Numerical::is_between(pointOfIntersection.x, left(), right()) && Numerical::is_between(pointOfIntersection.y, top(), bottom()))
			return make_intersection<Voxel::Intersected>(intersection->get_distance_from_origin(), Face::BACK);

		return make_intersection<Voxel::Intersected>();
	}

	Voxel::Intersected::Intersected(Face face)
		:_face(face)
	{}

	Voxel::Face Voxel::Intersected::get_face() const
	{
		return _face;
	}
}