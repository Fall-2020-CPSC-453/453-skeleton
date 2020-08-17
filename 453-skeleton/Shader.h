#pragma once

#include <GL/glew.h>

#include <string>

class Shader {

public:
	Shader(std::string vertexPath, std::string fragmentPath);

	// Copying not allowed
	Shader(const Shader&) = delete;
	Shader operator=(const Shader&) = delete;

	// Moving is allowed
	Shader(Shader&& other);
	Shader& operator=(Shader&& other);

	// Destructor to cleanup resources on GPU
	~Shader();

	// Public interface
	bool recompile();
	void use() const;

private:
	GLuint compileShader(std::string path, GLenum type);
	bool checkForLinkErrors(std::string message);

	GLint programID;

	GLuint vertexID;
	GLuint fragmentID;

	std::string vertexPath;
	std::string fragmentPath;
};

