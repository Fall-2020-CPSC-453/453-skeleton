#pragma once

#include "Shader.h"
#include "GLHandles.h"

#include <GL/glew.h>

#include <string>


class ShaderLinkException : public std::runtime_error {
public:
	explicit ShaderLinkException(const char* message)
		: std::runtime_error(message)
      {}

	explicit ShaderLinkException(const std::string& message)
		: std::runtime_error(message)
      {}
};

class ShaderProgram {

public:
	ShaderProgram(std::string vertexPath, std::string fragmentPath);

	// Public interface
	bool recompile();
	void use() const;

	void attach(GLuint shaderID) {
		glAttachShader(programID, shaderID);
	}


private:
	bool checkLinkSuccess(GLuint ID);

	ProgramID programID;

	Shader vertex;
	Shader fragment;
};
