#pragma once

#include "GLHandles.h"

#include <GL/glew.h>

#include <string>

class ShaderProgram;

class Shader {

public:
	Shader(const std::string& path, GLenum type);

	// Copying not allowed
	Shader(const Shader&) = delete;
	Shader operator=(const Shader&) = delete;

	// Moving is allowed
	Shader(Shader&& other) noexcept = default;
	Shader& operator=(Shader&& other) noexcept = default;

	// Destructor to cleanup resources on GPU
	~Shader() = default;


	// Public interface
	std::string getPath() const { return path; }
	GLenum getType() const { return type; }

	void friend attach(ShaderProgram& sp, Shader& s);

private:
	ShaderHandle shaderID;
	GLenum type;

	std::string path;

	bool compile();
};

