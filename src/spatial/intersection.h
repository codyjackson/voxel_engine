#pragma once

#include <boost/optional/optional.hpp>

class SimpleNakedIntersection
{
public:
	inline SimpleNakedIntersection(float distanceFromOrigin)
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
class NakedIntersection : public SimpleNakedIntersection
{
public:
	NakedIntersection(float distanceFromOrigin, const INTERSECTED_TYPE& intersected)
		:SimpleNakedIntersection(distanceFromOrigin), _intersected(intersected)
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
using SimpleIntersection = boost::optional<SimpleNakedIntersection>;

template<typename INTERSECTED_TYPE>
inline Intersection<INTERSECTED_TYPE> make_intersection(float distanceFromOrigin, const INTERSECTED_TYPE& intersected)
{
	return Intersection<INTERSECTED_TYPE>(NakedIntersection<INTERSECTED_TYPE>(distanceFromOrigin, intersected));
}

template<typename INTERSECTED_TYPE>
inline Intersection<INTERSECTED_TYPE> make_intersection()
{
	return Intersection<INTERSECTED_TYPE>();
}

inline SimpleIntersection make_simple_intersection(float distanceFromOrigin)
{
	return SimpleIntersection(SimpleNakedIntersection(distanceFromOrigin));
}

inline SimpleIntersection make_simple_intersection()
{
	return SimpleIntersection();
}

