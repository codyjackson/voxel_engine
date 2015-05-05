#pragma once

#include <boost/noncopyable.hpp>

template<typename T>
class Binder : boost::noncopyable
{
public:
	Binder(const T& bound)
		:_bound(bound)
	{
		_bound.bind();
	}

	~Binder()
	{
		_bound.unbind();
	}

private:
	const T& _bound;
};