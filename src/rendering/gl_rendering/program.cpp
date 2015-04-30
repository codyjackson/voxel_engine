#include "program.h"

#include "error.h"
#include "shader.h"

#include <GL/glew.h>
#include <glfw/glfw3.h>


Program::Program(const boost::filesystem::path& vertexPath, const boost::filesystem::path& fragmentPath)
:_id(glCreateProgram())
{
	gl_error_check();

	VertexShader vertexShader(vertexPath);
	FragmentShader fragmentShader(fragmentPath);

	glAttachShader(_id, vertexShader.getId()); gl_error_check();
	glAttachShader(_id, fragmentShader.getId()); gl_error_check();
	glLinkProgram(_id); gl_error_check();

	GLint linked = GL_FALSE;
	glGetProgramiv(_id, GL_LINK_STATUS, &linked); gl_error_check();
	if (linked == GL_FALSE) {
		int errorLength = 0;
		glGetProgramiv(_id, GL_INFO_LOG_LENGTH, &errorLength); gl_error_check();
		std::vector<char> error(errorLength, 0);
		glGetProgramInfoLog(_id, errorLength, NULL, error.data()); gl_error_check();
		throw LinkError("There was an error linking the shader program:\n" + std::string(std::begin(error), std::end(error)));
	}

}

Program::~Program()
{
	if (_id) {
		glDeleteProgram(_id); gl_error_check();
	}
}

void Program::bind() const
{
	glUseProgram(_id); gl_error_check();
}

void Program::unbind() const
{
	glUseProgram(0); gl_error_check();
}

Program::LinkError::LinkError(const std::string& error)
:std::runtime_error(error)
{}