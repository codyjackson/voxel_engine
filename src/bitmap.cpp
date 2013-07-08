#include "bitmap.h"

Bitmap::Bitmap()
{
	width = 1024;
	height = 768;
	Color red(255,0,0);
	Color black;
	for(int i = 0; i < width*height; i++)
	{
		pixels.push_back(red);
	}
}

Bitmap::~Bitmap()
{
	pixels.clear();
}

const int Bitmap::get_width() const
{
	return width;
}

const int Bitmap::get_height() const
{
	return height;
}

const std::vector<Color> Bitmap::get_pixels() const
{
	return pixels;
}
