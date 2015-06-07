#pragma once

#include "shader.h"

#include "../../utility/filesystem.h"

#include <boost/filesystem.hpp>

typedef unsigned int GLuint;

namespace Impl {
	class ShaderProgram;
}

class ShaderProgram : boost::noncopyable
{
public:
	ShaderProgram(const boost::filesystem::path& vertexPath, const boost::filesystem::path& fragmentPath);
	ShaderProgram(const boost::filesystem::path& vertexPath, const boost::filesystem::path& fragmentPath, bool liveReload);
	~ShaderProgram();

	void bind() const;
	void unbind() const;

	struct LinkError : public std::runtime_error
	{
		LinkError(const std::string& error);
	};

private:
	void reload();

	std::shared_ptr<Impl::ShaderProgram> _program;
	std::shared_ptr<Filesystem::FileWatcher> _vertexWatcher;
	std::shared_ptr<Filesystem::FileWatcher> _fragmentWatcher;

};

