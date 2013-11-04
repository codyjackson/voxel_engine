#include "color.h"

const Color Color::INVISIBLE(0,0,0,0);

Color::Color(): _red(255), _green(0), _blue(0)
{

}

Color::Color(const char red, const char green, const char blue, const char alpha)
	:_red(red), _green(green), _blue(blue), _alpha(alpha)
{

}

Color::~Color()
{

}

const unsigned char Color::get_red() const
{
	return _red;
}

const unsigned char Color::get_green() const
{
	return _green;
}

const unsigned char Color::get_blue() const
{
	return _blue;
}

const Color Color::operator=( Color rhs)
{
	this->_red = rhs.get_red();
	this->_green = rhs.get_green();
	this->_blue = rhs.get_blue();
	return *this;
}