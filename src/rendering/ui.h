#pragma once

#include "color.h"

#include <vector>


typedef unsigned int GLuint;

class UI
{
public:
	UI(int width, int height);
	~UI();

	void tick();

private:
	void render(const void* buffer);

	int _width;
	int _height;
	std::vector<Color> _pixels;
	GLuint _textureHandle;
};