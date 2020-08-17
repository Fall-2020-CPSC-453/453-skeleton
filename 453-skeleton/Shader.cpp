#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

Shader::Shader(std::string vertexPath, std::string fragmentPath) :
	vertexPath(vertexPath),
	fragmentPath(fragmentPath)
{

	programID = glCreateProgram();

	vertexID = compileShader(vertexPath, GL_VERTEX_SHADER);
	fragmentID = compileShader(fragmentPath, GL_FRAGMENT_SHADER);

	if (vertexID && fragmentID) {
		glAttachShader(programID, vertexID);
		glAttachShader(programID, fragmentID);
		glLinkProgram(programID);

		// check for linking errors
		valid = checkForLinkErrors(vertexPath + " and " + fragmentPath);
	}
	
	// flag shaders for deletion as they are linked into the program and no longer necessary
	glDeleteShader(vertexID);
	glDeleteShader(fragmentID);
}


bool Shader::recompile() {
	
	GLuint newVertexID = compileShader(vertexPath, GL_VERTEX_SHADER);
	GLuint newFragmentID = compileShader(vertexPath, GL_FRAGMENT_SHADER);

	if (newVertexID && newFragmentID) {

		glDetachShader(programID, vertexID);
		glDetachShader(programID, fragmentID);

		vertexID = newVertexID;
		fragmentID = newFragmentID;
		
		glAttachShader(programID, vertexID);
		glAttachShader(programID, fragmentID);
		glLinkProgram(programID);

		// check for linking errors
		valid = checkForLinkErrors(vertexPath + " and " + fragmentPath);
	}

	// flag shaders for deletion as they are linked into the program and no longer necessary
	glDeleteShader(vertexID);
	glDeleteShader(fragmentID);
	
	return valid;
}


bool Shader::use() {
	if (valid) {
		glUseProgram(programID);
	}
	else {
		std::cout << "ERROR::SHADER trying to make active invalid shader program" << std::endl;
	}
	return valid;
}


GLuint Shader::compileShader(std::string path, GLenum type) {

	// ******************
	// read shader source
	// ******************
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


	// **************
	// compile shader
	// **************
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
