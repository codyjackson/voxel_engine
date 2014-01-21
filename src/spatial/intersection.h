#pragma once

template<typename OBJECT_OF_INTEREST_TYPE>
class Intersection
{
public:
	Intersection()
		:_intersected(false)
	{}

	Intersection(const glm::vec3& pointOfIntersection, const OBJECT_OF_INTEREST_TYPE& intersectedObject)
		:_intersected(true), _pointOfIntersection(pointOfIntersection), _intersectedObject(intersectedObject)
	{}

	bool operator bool() const
	{
		return _intersected;
	}

	const glm::vec3& get_point_of_intersection() const
	{
		return _intersected;
	}

	glm::vec3& get_point_of_intersection()
	{
		return _intersected;
	}

	const OBJECT_OF_INTEREST_TYPE& get_object_of_interest() const
	{
		return _objectOfInterest;
	}

	OBJECT_OF_INTEREST_TYPE& get_object_of_interest()
	{
		return _objectOfInterest;
	}

private:
	bool _intersected;
	float _distanceFromOrigin;
	glm::vec3 _pointOfIntersection;
	OBJECT_OF_INTEREST_TYPE& _objectOfInterest;
};