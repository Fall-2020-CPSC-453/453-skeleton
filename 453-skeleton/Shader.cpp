#include "Shader.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>


Shader::Shader(std::string vertexPath, std::string fragmentPath) :
	vertexPath(vertexPath),
	fragmentPath(fragmentPath)
{
	bool success = false;

	programID = glCreateProgram();

	vertexID = compileShader(vertexPath, GL_VERTEX_SHADER);
	fragmentID = compileShader(fragmentPath, GL_FRAGMENT_SHADER);

	if (vertexID && fragmentID) {
		glAttachShader(programID, vertexID);
		glAttachShader(programID, fragmentID);
		glLinkProgram(programID);

		// check for linking errors
		success = checkForLinkErrors(vertexPath + " and " + fragmentPath);
	}

	if (!success) {
		glDeleteShader(vertexID);
		glDeleteShader(fragmentID);
		glDeleteProgram(programID);

		throw std::runtime_error("Shaders did not compile and/or link. Cannot complete construction of object.");
	}
}


Shader::Shader(Shader&& other) :
	programID(std::move(other.programID)),
	vertexID(std::move(other.vertexID)),
	fragmentID(std::move(other.fragmentID)),
	vertexPath(std::move(other.vertexPath)),
	fragmentPath(std::move(other.fragmentPath))
{
	other.programID = 0;
	other.vertexID = 0;
	other.fragmentID = 0;
}


Shader& Shader::operator=(Shader&& other) {
	programID = std::move(other.programID);
	vertexID = std::move(other.vertexID);
	fragmentID = std::move(other.fragmentID);
	vertexPath = std::move(other.vertexPath);
	fragmentPath = std::move(other.fragmentPath);

	other.programID = 0;
	other.vertexID = 0;
	other.fragmentID = 0;

	return *this;
}


Shader::~Shader() {
	glDeleteShader(vertexID);
	glDeleteShader(fragmentID);
	glDeleteProgram(programID);
}


bool Shader::recompile() {
	
	GLuint newVertexID = compileShader(vertexPath, GL_VERTEX_SHADER);
	GLuint newFragmentID = compileShader(fragmentPath, GL_FRAGMENT_SHADER);

	// both shaders compiled, try linking
	if (newVertexID && newFragmentID) {

		glDetachShader(programID, vertexID);
		glDetachShader(programID, fragmentID);

		glAttachShader(programID, newVertexID);
		glAttachShader(programID, newFragmentID);
		glLinkProgram(programID);

		// check for linking errors
		bool success = checkForLinkErrors(vertexPath + " and " + fragmentPath);

		if (success) {
			glDeleteShader(vertexID);
			glDeleteShader(fragmentID);

			vertexID = newVertexID;
			fragmentID = newFragmentID;
			return true;
		}
		else {
			std::cerr << "INFO::SHADER falling back to previous version of shaders" << std::endl;
			glDetachShader(programID, newVertexID);
			glDetachShader(programID, newFragmentID);

			glAttachShader(programID, vertexID);
			glAttachShader(programID, fragmentID);
			glLinkProgram(programID);

			// check for linking errors
			if (!checkForLinkErrors(vertexPath + " and " + fragmentPath)) {
				throw std::runtime_error("Previous shaders did not compile and/or link. Should not happen.");
			}
			return false;
		}
	}
	else {
		return false;
	}
}


void Shader::use() const {
	glUseProgram(programID);
}


GLuint Shader::compileShader(std::string path, GLenum type) {

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
	catch (std::ifstream::failure e) {
		std::cerr << "ERROR::SHADER reading " << path << ":" << std::endl;
		std::cerr << strerror(errno) << std::endl;
		return 0;
	}
	const GLchar* sourceCode = sourceString.c_str();


	// compile shader
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &sourceCode, NULL);
	glCompileShader(shader);

	// check for errors
	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success) {
		GLint logLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<char> log(logLength);
		glGetShaderInfoLog(shader, logLength, NULL, log.data());

		std::cerr << "ERROR::SHADER compiling " << path << ":" << std::endl;
		std::cerr << log.data() << std::endl;
		return 0;
	}

	return shader;
}

bool Shader::checkForLinkErrors(std::string message) {

	GLint success;

	// check for link errors
	glGetProgramiv(programID, GL_LINK_STATUS, &success);
	if (!success) {
		GLint logLength;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<char> log(logLength);
		glGetProgramInfoLog(programID, logLength, NULL, log.data());

		std::cerr << "ERROR::SHADER linking " << message << ":" << std::endl;
		std::cerr << log.data() << std::endl;

		return false;
	}
	else {
		std::cerr << "INFO::SHADER successfully compiled and linked " << message << std::endl;
		return true;
	}
}
