#pragma once

#include <boost/optional/optional.hpp>

class SimpleIntersection
{
public:
	inline SimpleIntersection(float distanceFromOrigin)
		:_distanceFromOrigin(distanceFromOrigin)
	{}

	inline float get_distance_from_origin() const
	{
		return _distanceFromOrigin;
	}

private:
	float _distanceFromOrigin;
};

template<typename INTERSECTED_TYPE>
class NakedIntersection : public SimpleIntersection
{
public:
	NakedIntersection(float distanceFromOrigin, const INTERSECTED_TYPE& intersected)
		:SimpleIntersection(distanceFromOrigin), _intersected(intersected)
	{}

	NakedIntersection()
	{}

	const INTERSECTED_TYPE get_object_of_interest() const
	{
		return _intersected;
	}

	INTERSECTED_TYPE get_object_of_interest()
	{
		return _intersected;
	}

private:
	float _distanceFromOrigin;
	INTERSECTED_TYPE _intersected;
};

template<typename INTERSECTED_TYPE>
using Intersection = boost::optional<NakedIntersection<INTERSECTED_TYPE>>;

template<typename INTERSECTED_TYPE>
inline Intersection<INTERSECTED_TYPE> make_intersection(float distanceFromOrigin, const INTERSECTED_TYPE& intersected)
{
	return Intersection<INTERSECTED_TYPE>(NakedIntersection<INTERSECTED_TYPE>(distanceFromOrigin, intersected));
}
