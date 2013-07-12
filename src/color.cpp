#include "color.h"

Color::Color(): red(255), green(0), blue(0)
{

}

Color::Color(const char red, const char green, const char blue)
	:red(red), green(green), blue(blue)
{

}

Color::~Color()
{

}

const unsigned char Color::get_red() const
{
	return red;
}

const unsigned char Color::get_green() const
{
	return green;
}

const unsigned char Color::get_blue() const
{
	return blue;
}

const Color Color::operator=( Color rhs)
{
	this->red = rhs.get_red();
	this->green = rhs.get_green();
	this->blue = rhs.get_blue();
	return *this;
}