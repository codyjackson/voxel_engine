#include "program.h"

#include "shader.h"

#include <GL/glew.h>
#include <glfw/glfw3.h>

Program::Program(const boost::filesystem::path& vertexPath, const boost::filesystem::path& fragmentPath)
:_id(glCreateProgram())
{
	VertexShader vertexShader(vertexPath);
	FragmentShader fragmentShader(fragmentPath);

	glAttachShader(_id, vertexShader.getId());
	glAttachShader(_id, fragmentShader.getId());
	glLinkProgram(_id);

	GLint linked = GL_FALSE;
	glGetProgramiv(_id, GL_LINK_STATUS, &linked);
	if (linked == GL_FALSE) {
		int errorLength = 0;
		glGetProgramiv(_id, GL_INFO_LOG_LENGTH, &errorLength);
		std::vector<char> error(errorLength, 0);
		glGetProgramInfoLog(_id, errorLength, NULL, error.data());
		throw LinkError("There was an error linking the shader program:\n" + std::string(std::begin(error), std::end(error)));
	}

}

Program::~Program()
{
	if (_id) {
		glDeleteProgram(_id);
	}
}

Program::LinkError::LinkError(const std::string& error)
:std::runtime_error(error)
{}