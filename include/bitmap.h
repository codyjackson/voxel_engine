#pragma once

#include<vector>

struct Color
{
	char red;
	char green;
	char blue;
};

class Bitmap
{
	public:
		Bitmap();
		Bitmap(const int width, const int height, const Color color);
		~Bitmap();
		void set_pixel(int x, int y, char color);
		const int get_width() const;
		const int get_height() const;
		const Color* get_pixels() const;
	private:
		Color* pixels;
		int width;
		int height;
};