#include "ui.h"

#include <Windows.h>
#include <GLFW/glfw3.h>

UI::UI(int width, int height)
:_pixels(width*height, Color(255, 0, 0))
{
	glGenTextures(1, &_textureHandle);
	auto error = glGetError();
	glBindTexture(GL_TEXTURE_2D, _textureHandle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, static_cast<void*>(&_pixels));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, NULL);
}

UI::~UI()
{
	glDeleteTextures(1, &_textureHandle);
}

void UI::render() const
{
	glBindTexture(GL_TEXTURE_2D, _textureHandle);
	glLoadIdentity();
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(0.0, 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(0.0f, 1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, 1.0f);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, NULL);
}
