#include "color.h"

Color::Color()
{
	red = 255;
	green = 0;
	blue = 0;
}

Color::Color(const char red, const char green, const char blue)
{
	this->red = red;
	this->green = green;
	this->blue = blue;
}

Color::~Color()
{

}

const char Color::get_red() const
{
	return red;
}

const char Color::get_green() const
{
	return green;
}

const char Color::get_blue() const
{
	return blue;
}