#pragma once

#include "shader.h"

#include <boost/filesystem.hpp>

typedef unsigned int GLuint;

class Program
{
public:
	Program(const boost::filesystem::path& vertexPath, const boost::filesystem::path& fragmentPath);
	~Program();

	void bind() const;
	void unbind() const;

	struct LinkError : public std::runtime_error
	{
		LinkError(const std::string& error);
	};

private:
	VertexShader _vertexShader;
	FragmentShader _fragmentShader;
	GLuint _id;
};

