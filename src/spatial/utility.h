#pragma once

#include <iterator>

namespace Spatial
{
	namespace Utility
	{
		template<typename T>
		typename T::value_type get_nearest_intersection(const T& intersections)
		{
			const auto reduceIfNearer = [](std::pair<float, T::value_type> currentNearest, const T::value_type& intersection){
				if (!intersection)
					return currentNearest;
				const float distance = intersection->get_distance_from_origin();
				return distance < currentNearest.first ? std::make_pair(distance, intersection) : currentNearest;
			};
			const auto seed = std::make_pair(std::numeric_limits<float>::max(), make_intersection<T::value_type::value_type::intersected_type>());
			const auto nearest = Functional::reduce(std::begin(intersections), std::end(intersections), seed, reduceIfNearer);
			return nearest.second;
		}
	}
}