#pragma once

#include "Shader.h"

#include <GL/glew.h>

#include <string>


class ShaderProgram {

public:
	ShaderProgram(std::string vertexPath, std::string fragmentPath);

	// Copying not allowed
	ShaderProgram(const ShaderProgram&) = delete;
	ShaderProgram operator=(const ShaderProgram&) = delete;

	// Moving is allowed
	ShaderProgram(ShaderProgram&& other);
	ShaderProgram& operator=(ShaderProgram&& other);

	// Destructor to cleanup resources on GPU
	~ShaderProgram();
	void dealloc();


	// Public interface
	bool recompile();
	void use() const;

private:
	bool checkLinkSuccess(GLuint ID);

	GLint programID;

	Shader vertex;
	Shader fragment;
};
