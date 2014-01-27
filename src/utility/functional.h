#pragma once

#include <functional>

namespace Functional
{
	template<typename INPUTER_ITERATOR, typename RETURN_TYPE, typename FUNCTION_TYPE>
	RETURN_TYPE reduce(INPUTER_ITERATOR begin, INPUTER_ITERATOR end, RETURN_TYPE initialValue, const FUNCTION_TYPE& f)
	{
		while (begin != end) {
			initialValue = f(initialValue, *begin);
			++begin;
		}
		return initialValue;
	}
}