#include "bitmap.h"

Bitmap::Bitmap(): width(1024), height(720), pixels(1024*720, Color())
{

}
Bitmap::Bitmap(const int width, const int height, const Color color)
	:width(width), height(height), pixels(width*height, color)
{

}

Bitmap::~Bitmap()
{
	
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
