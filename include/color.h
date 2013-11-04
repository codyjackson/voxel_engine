#pragma once

class Color
{
	public:
		Color();
		Color(const char red, const char green, const char blue, const char alpha);
		~Color();
		const unsigned char get_red() const;
		const unsigned char get_green() const;
		const unsigned char get_blue() const;
		const unsigned char get_alpha() const;
		const Color operator=( Color rhs);

		static const Color INVISIBLE;

	private:
		unsigned char _red;
		unsigned char _green;
		unsigned char _blue;
		unsigned char _alpha;
};