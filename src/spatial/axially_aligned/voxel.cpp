#include "voxel.h"

#include "../../constants/vector.h"
#include "../../utility/numerical.h"

namespace AxiallyAligned
{
	Voxel::Voxel(const glm::vec3& topLeftFront, float sideLength)
		:_topLeftFront(topLeftFront), _sideLength(sideLength)
	{}

<<<<<<< HEAD
	glm::vec3 Voxel::top_right_front() const
	{
		return glm::vec3 (right(), top(), front());
	}
	glm::vec3 Voxel::top_right_back() const
	{
		return glm::vec3(right(), top(), back());
	}
	glm::vec3 Voxel::top_left_back() const
	{
		return glm::vec3(left(), top(), back());
	}
	glm::vec3 Voxel::top_left_front() const
	{
		return _topLeftFront;
	}
	glm::vec3 Voxel::bot_left_front() const
	{
		return glm::vec3(left(), bottom(), front());
	}
	glm::vec3 Voxel::bot_right_front() const
	{
		return glm::vec3(right(), bottom(), front());
	}
	glm::vec3 Voxel::bot_left_back() const
	{
		return glm::vec3(left(), bottom(), back());
	}
	glm::vec3 Voxel::bot_right_back() const
	{
		return glm::vec3(right(), bottom(), back());
	}

	bool Voxel::is_inside(const glm::vec3& p ) const
	{
		return is_between(p.x, left(), right()) && is_between(p.y, top(), bottom()) && is_between(p.z, front(), back());
	}

	bool Voxel::does_intersect(const Voxel& a) const
	{
		return is_inside(a.bot_left_back()) || is_inside(a.bot_left_front()) || is_inside(a.bot_right_back()) || is_inside(a.bot_right_front()) || is_inside(a.top_left_back()) || is_inside(a.top_left_front()) || is_inside(a.top_right_back()) || is_inside(a.top_right_front());
	}

	Intersection<Voxel> Voxel::find_intersection(const Ray& r) const
=======
	Intersection<Voxel::Intersected> Voxel::find_intersection(const Ray& r) const
>>>>>>> upstream/master
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