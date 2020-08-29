#pragma once

#include "Shader.h"

#include <GL/glew.h>

#include <string>


class ShaderProgram {

public:
	ShaderProgram(const std::string& vertexPath, const std::string& fragmentPath);

	// Copying not allowed
	ShaderProgram(const ShaderProgram&) = delete;
	ShaderProgram operator=(const ShaderProgram&) = delete;

	// Moving is allowed
	ShaderProgram(ShaderProgram&& other) noexcept;
	ShaderProgram& operator=(ShaderProgram&& other) noexcept;

	// Destructor to cleanup resources on GPU
	~ShaderProgram();


	// Public interface
	bool recompile();
	void use() const { glUseProgram(programID); }

	void friend attach(ShaderProgram& sp, Shader& s);

private:
	GLint programID;

	Shader vertex;
	Shader fragment;

	bool checkAndLogLinkSuccess() const;
};
