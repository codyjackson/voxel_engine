#include "color.h"

const Color Color::INVISIBLE(0,0,0,0);

Color::Color()
	: _red(255), _green(0), _blue(0), _alpha(0)
{}

Color::Color(const unsigned char red, const unsigned char green, const unsigned char blue)
	:Color(red, green, blue, 255)
{}

Color::Color(const unsigned char red, const unsigned char green, const unsigned char blue, const unsigned char alpha)
	:_red(red), _green(green), _blue(blue), _alpha(alpha)
{}

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

const unsigned char Color::get_alpha() const
{
	return _alpha;
}

const Color Color::operator=( Color rhs)
{
	this->_red = rhs._red;
	this->_green = rhs._green;
	this->_blue = rhs._blue;
	this->_alpha = rhs._alpha;
	return *this;
}