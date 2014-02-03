#pragma once

#include <iterator>

namespace Spatial
{
	namespace Utility
	{
		template<typename INTERSECTION_INTERATOR_TYPE>
		typename std::iterator_traits<INTERSECTION_INTERATOR_TYPE>::value_type get_nearest_intersection(INTERSECTION_INTERATOR_TYPE begin, INTERSECTION_INTERATOR_TYPE end)
		{
			const auto findNearest = [](std::pair<float, std::iterator_traits<INTERSECTION_INTERATOR_TYPE>::value_type> x, const std::iterator_traits<INTERSECTION_INTERATOR_TYPE>::value_type& intersection){
				if (!intersection)
					return x;
				const float distance = intersection->get_distance_from_origin();
				return distance < x.first ? std::make_pair(distance, intersection) : x;
			};
			const auto seed = std::make_pair(std::numeric_limits<float>::max(), make_intersection<std::iterator_traits<INTERSECTION_INTERATOR_TYPE>::value_type::value_type::intersected_type>());
			const auto nearest = Functional::reduce(begin, end, seed, findNearest);
			return nearest.second;
		}
	}
}