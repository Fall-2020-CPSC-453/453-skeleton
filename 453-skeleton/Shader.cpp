#include "Shader.h"

#include "Log.h"

#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>


Shader::Shader(const std::string& path, GLenum type)
	: type(type)
	, path(path)
{
	if (!compile()) {
		throw std::runtime_error("Shader did not compile");
	}
}


Shader::Shader(Shader&& other) noexcept
	: shaderID(std::move(other.shaderID))
	, type(std::move(other.type))
	, path(std::move(other.path))
{
	other.shaderID = 0;
}


Shader& Shader::operator=(Shader&& other) noexcept {

	this->~Shader();

	shaderID = std::move(other.shaderID);
	type = std::move(other.type);
	path = std::move(other.path);

	other.shaderID = 0;
	return *this;
}


Shader::~Shader() {
	glDeleteShader(shaderID);
}


bool Shader::compile() {

	// read shader source
	std::string sourceString;
	std::ifstream file;

	// ensure ifstream objects can throw exceptions:
	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		// open file
		file.open(path);
		std::stringstream sourceStream;

		// read file buffer contents into stream
		sourceStream << file.rdbuf();

		// close file handler
		file.close();

		// convert stream into string
		sourceString = sourceStream.str();
	}
	catch (std::ifstream::failure &e) {
		Log::error("SHADER reading {}:\n{}", path, strerror(errno));
		return false;
	}
	const GLchar* sourceCode = sourceString.c_str();


	// compile shader
	shaderID = glCreateShader(type);
	glShaderSource(shaderID, 1, &sourceCode, NULL);
	glCompileShader(shaderID);

	// check for errors
	GLint success;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

	if (!success) {
		GLint logLength;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<char> log(logLength);
		glGetShaderInfoLog(shaderID, logLength, NULL, log.data());

		Log::error("SHADER compiling {}:\n{}", path, log.data());
	}
	return success;
}
