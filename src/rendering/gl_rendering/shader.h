#pragma once

#include <boost/filesystem.hpp>

#include <exception>
#include <string>

typedef unsigned int GLuint;

namespace Impl {
	class ShaderProgram;
}

class Shader : public boost::noncopyable
{
public:
	enum class Type
	{
		VERTEX,
		FRAGMENT
	};

	struct CompilationError : public std::runtime_error
	{
		CompilationError(const boost::filesystem::path& path, const std::string& error);
	};

	Shader(Type type, boost::filesystem::path path);
	~Shader();

private:
	friend class Impl::ShaderProgram;
	GLuint getId() const;

	GLuint _id;
};

class VertexShader : public Shader
{
public:
	VertexShader(boost::filesystem::path path);
};

class FragmentShader : public Shader
{
public:
	FragmentShader(boost::filesystem::path path);
};