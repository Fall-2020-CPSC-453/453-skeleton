#pragma once

#include <GL/glew.h>

#include <string>

class Shader {

public:
	Shader(std::string vertexPath, std::string fragmentPath);

	bool recompile();

	bool use();

private:
	GLuint compileShader(std::string path, GLenum type);
	bool checkForLinkErrors(std::string message);

	GLint programID;
	bool valid;

	std::string vertexPath;
	std::string fragmentPath;

	GLuint vertexID;
	GLuint fragmentID;
};

