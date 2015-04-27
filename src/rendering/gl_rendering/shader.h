#pragma once

#include <boost/filesystem.hpp>

#include <exception>
#include <string>

typedef unsigned int GLuint;

class Shader
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
	friend class Program;
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