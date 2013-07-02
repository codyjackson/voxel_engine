#include "bitmap.h"

Bitmap::Bitmap()
{
	width = 1024;
	height = 768;
	pixels = new Color[width *height];
	for(int i = 0; i < width*height; i++)
	{
		pixels[i].red = i%120;
		pixels[i].green = 0;
		pixels[i].blue = 0;
	}
}

Bitmap::~Bitmap()
{
	delete [] pixels;
}

const int Bitmap::get_width() const
{
	return width;
}

const int Bitmap::get_height() const
{
	return height;
}

const Color* Bitmap::get_pixels() const
{
	return pixels;
}
