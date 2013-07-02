#pragma once

class Color
{
	public:
		Color();
		Color(const char red, const char green, const char blue);
		~Color();
		const char get_red() const;
		const char get_green() const;
		const char get_blue() const;
	private:
		char red;
		char green;
		char blue;
};