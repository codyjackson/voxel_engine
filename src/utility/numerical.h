#pragma once

namespace Numerical
{
	template<typename T>
	bool is_between(T checkedValue, T first, T second)
	{
		if (first > second) {
			std::swap(first, second);
		}
		return (checkedValue >= first) && (checkedValue <= second);
	}
}