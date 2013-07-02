#include "bitmap.h"

Bitmap::Bitmap()
{
	width = 640;
	height = 480;
	pixels = new Color[width *height];
	for(int i = 0; i < width*height; i++)
	{
		pixels[i].red = 'A';
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
