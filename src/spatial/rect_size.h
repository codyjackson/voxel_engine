#pragma once

class RectSize
{
public:
	RectSize(int width, int height)
		:width(width), height(height)
	{}

	int get_area() const
	{
		return width * height;
	}

	int width;
	int height;
};

inline bool operator==(const RectSize& lhs, const RectSize& rhs)
{
	return (lhs.width == rhs.width) && (lhs.height == rhs.height);
}

inline bool operator!=(const RectSize& lhs, const RectSize& rhs)
{
	return !(lhs == rhs);
}