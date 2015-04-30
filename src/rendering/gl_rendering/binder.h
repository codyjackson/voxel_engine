#pragma once

template<typename T>
class Binder
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