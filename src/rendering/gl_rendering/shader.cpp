#include "shader.h"
#include "../../utility/filesystem.h"

#include <GL/glew.h>
#include <glfw/glfw3.h>

#include <exception>

namespace
{
	GLuint toGl(Shader::Type type)
	{
		switch (type) {
		case Shader::Type::VERTEX:
			return GL_VERTEX_SHADER;
		default:
			return GL_FRAGMENT_SHADER;
		};
	}
}

Shader::Shader(Type type, boost::filesystem::path path)
:_id(glCreateShader(toGl(type)))
{
	std::string fileContents = Filesystem::get_file_contents(path);
	const char* contentsPointer = fileContents.c_str();
	glShaderSource(_id, 1, &contentsPointer, NULL);
	glCompileShader(_id);

	GLint compiled = GL_FALSE;
	glGetShaderiv(_id, GL_COMPILE_STATUS, &compiled);
	if (compiled == GL_FALSE) {
		int errorLength = 0;
		glGetShaderiv(_id, GL_INFO_LOG_LENGTH, &errorLength);
		std::vector<char> error(errorLength, 0);
		glGetShaderInfoLog(_id, errorLength, NULL, error.data());
		throw CompilationError(path, "There was an error compiling " + path.string() + ":\n" + std::string(std::begin(error), std::end(error)));
	}
}

Shader::~Shader()
{
	if (_id) {
		glDeleteShader(_id);
	}
}

GLuint Shader::getId() const
{
	return _id;
}

Shader::CompilationError::CompilationError(const boost::filesystem::path& path, const std::string& error)
:std::runtime_error("Error compiling: " + path.string() + "\n " + error)
{}

VertexShader::VertexShader(boost::filesystem::path path)
: Shader(Shader::Type::VERTEX, path)
{}

FragmentShader::FragmentShader(boost::filesystem::path path)
: Shader(Shader::Type::FRAGMENT, path)
{}