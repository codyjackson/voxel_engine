#include "program.h"

#include "error.h"
#include "shader.h"

#include <GL/glew.h>
#include <glfw/glfw3.h>
namespace Impl
{
	class ShaderProgram : boost::noncopyable
	{
	public:
		ShaderProgram(const boost::filesystem::path& vertexPath, const boost::filesystem::path& fragmentPath)
			:_vertexShader(std::make_shared<VertexShader>(vertexPath)), _fragmentShader(std::make_shared<FragmentShader>(fragmentPath)), _id(glCreateProgram())
		{
			gl_error_check();

			glAttachShader(_id, _vertexShader->getId()); gl_error_check();
			glAttachShader(_id, _fragmentShader->getId()); gl_error_check();
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

		~ShaderProgram()
		{
			if (_id) {
				glDetachShader(_id, _vertexShader->getId()); gl_error_check();
				glDetachShader(_id, _fragmentShader->getId()); gl_error_check();
				glDeleteProgram(_id); gl_error_check();
			}
		}

		void bind() const
		{
			glUseProgram(_id); gl_error_check();
		}

		void unbind() const
		{
			glUseProgram(0); gl_error_check();
		}

		struct LinkError : public std::runtime_error
		{
			LinkError(const std::string& error)
				:std::runtime_error(error)
			{}
		};

	private:
		std::shared_ptr<VertexShader> _vertexShader;
		std::shared_ptr<FragmentShader> _fragmentShader;
		GLuint _id;
	};
}

ShaderProgram::ShaderProgram(const boost::filesystem::path& vertexPath, const boost::filesystem::path& fragmentPath)
:ShaderProgram(vertexPath, fragmentPath, false)
{}

ShaderProgram::ShaderProgram(const boost::filesystem::path& vertexPath, const boost::filesystem::path& fragmentPath, bool liveReload)
: _program(std::make_shared<Impl::ShaderProgram>(vertexPath, fragmentPath))
{
	if (liveReload) {
		_vertexWatcher = std::make_shared<Filesystem::FileWatcher>(vertexPath, std::bind(&ShaderProgram::reload, this));
		_fragmentWatcher = std::make_shared<Filesystem::FileWatcher>(fragmentPath, std::bind(&ShaderProgram::reload, this));
	}
}

ShaderProgram::~ShaderProgram()
{
	
}

void ShaderProgram::reload()
{
	try {
		_program = std::make_shared<Impl::ShaderProgram>(_vertexWatcher->get_path(), _fragmentWatcher->get_path());
	}
	catch(const std::exception& ex) {
		std::cout << ex.what() << std::endl;
	}

}

void ShaderProgram::bind() const
{
	_program->bind();
}

void ShaderProgram::unbind() const
{
	_program->unbind();
}

ShaderProgram::LinkError::LinkError(const std::string& error)
:std::runtime_error(error)
{}