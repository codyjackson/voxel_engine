#pragma once

#include<vector>
#include "color.h"

class Bitmap
{
	public:
		Bitmap();
		Bitmap(const int width, const int height, const Color color);
		~Bitmap();
		void set_pixel(int x, int y, char color);
		const int get_width() const;
		const int get_height() const;
		const std::vector<Color> get_pixels() const;
	private:
		std::vector<Color> pixels;
		int width;
		int height;
};