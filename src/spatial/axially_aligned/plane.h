#pragma once

#include "../intersection.h"
#include "../ray.h"

namespace AxiallyAligned
{
	class Plane
	{
	public:
		enum class Span{ XY, XZ, YZ };
		Plane(Span span, float constant);

		Intersection<Plane> find_intersection(const Ray& r) const;

	private:
		Intersection<Plane> find_intersection_for_components(float originComponent, float directionComponent) const;

		Span _span;
		float _constant;
	};
}