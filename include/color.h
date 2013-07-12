#pragma once

class Color
{
	public:
		Color();
		Color(const char red, const char green, const char blue);
		~Color();
		const unsigned char get_red() const;
		const unsigned char get_green() const;
		const unsigned char get_blue() const;
		const Color operator=( Color rhs);
	private:
		unsigned char red;
		unsigned char green;
		unsigned char blue;
};