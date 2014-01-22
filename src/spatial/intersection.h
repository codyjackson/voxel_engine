#pragma once

#include <boost/optional/optional.hpp>

template<typename INTERSECTED_TYPE>
class NakedIntersection
{
public:
	NakedIntersection(float distanceFromOrigin, const INTERSECTED_TYPE& intersected)
		:_distanceFromOrigin(distanceFromOrigin), _intersected()
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

	float get_distance_from_origin() const
	{
		return _distanceFromOrigin;
	}



private:
	float _distanceFromOrigin;
	INTERSECTED_TYPE _intersected;
};

template<typename INTERSECTED_TYPE>
using Intersection = NakedIntersection<INTERSECTED_TYPE>;
