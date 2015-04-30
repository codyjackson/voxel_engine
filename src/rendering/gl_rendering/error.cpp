#include "error.h"

#include <stdexcept>

#include <GL/glew.h>
#include <GL/GL.h>

namespace
{
	std::string toGl(GLenum e)
	{
		if (e == GL_INVALID_ENUM) {
			return "";
		}

		switch (e) {
			case GL_NO_ERROR:
				return "GL_NO_ERROR";
			case GL_INVALID_ENUM:
				return "GL_INVALID_ENUM";
			case GL_INVALID_VALUE:
				return "GL_INVALID_VALUE";
			case GL_INVALID_OPERATION:
				return "GL_INVALID_OPERATION";
			case GL_INVALID_FRAMEBUFFER_OPERATION:
				return "GL_INVALID_FRAMEBUFFER_OPERATION";
			case GL_OUT_OF_MEMORY:
				return "GL_OUT_OF_MEMORY";
			case GL_STACK_UNDERFLOW:
				return "GL_STACK_UNDERFLOW";
			case GL_STACK_OVERFLOW:
				return "GL_STACK_OVERFLOW";
			default:
				throw std::runtime_error("Invalid enum specified.");
		}
	}

	struct GLError : public std::runtime_error
	{
		GLError(GLenum e)
			:std::runtime_error("There was an OpenGL error: " + toGl(e))
		{}
	};
}

void gl_error_check()
{
	GLenum error = glGetError();
	if (error == GL_NO_ERROR) {
		return;
	}

	throw GLError(error);
}