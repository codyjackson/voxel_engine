#include "plane.h"

namespace AxiallyAligned
{
	Plane::Plane(Span span, float constant)
		:_span(span), _constant(constant)
	{}

	Intersection<Plane> Plane::find_intersection(const Ray& r) const
	{
		if (_span == Span::XY)
			return find_intersection_for_components(r.origin().z, r.direction().z);
		if (_span == Span::XZ)
			return find_intersection_for_components(r.origin().y, r.direction().y);
		return find_intersection_for_components(r.origin().x, r.direction().x);
	}

	Intersection<Plane> Plane::find_intersection_for_components(float originComponent, float directionComponent) const
	{
		if (_constant == originComponent)
			return make_intersection(0.0f, *this);

		if (_constant == directionComponent)
			return Intersection<Plane>();

		if (!(directionComponent || _constant))
			return Intersection<Plane>();

		const float scalar = (_constant - originComponent) / directionComponent;
		return scalar < 0 ? Intersection<Plane>() : make_intersection<Plane>(scalar, *this);
	}
}