#pragma once

#include "browser/browser.h"

#include <vector>


typedef unsigned int GLuint;

class UI
{
public:
	UI(int width, int height);
	~UI();

	void tick();

private:
	int _width;
	int _height;

	GLuint _textureHandle;
	Browser _browser;
};