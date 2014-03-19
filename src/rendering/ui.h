#pragma once

#include "color.h"

#include <vector>

typedef unsigned int GLuint;
class UI
{
public:
	UI(int width, int height);
	~UI();

	void render() const;

private:
	std::vector<Color> _pixels;
	GLuint _textureHandle;
};