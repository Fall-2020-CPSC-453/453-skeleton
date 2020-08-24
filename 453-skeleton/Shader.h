#pragma once

#include "GLHandles.h"

#include <GL/glew.h>

#include <string>


class ShaderCompileException : public std::runtime_error {
public:
	explicit ShaderCompileException(const char* message)
		: std::runtime_error(message)
      {}

	explicit ShaderCompileException(const std::string& message)
		: std::runtime_error(message)
      {}
};


class Shader {

public:
	Shader(std::string path, GLenum type);

	// Public interface
	std::string getPath() const { return path; }
	GLenum getType() const { return type; }
	operator GLuint() const { return shaderID; }

private:
	ShaderID shaderID;
	GLenum type;

	std::string path;

	bool compile();
};

