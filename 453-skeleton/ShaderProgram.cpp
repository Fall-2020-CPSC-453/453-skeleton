#include "ShaderProgram.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>


ShaderProgram::ShaderProgram(std::string vertexPath, std::string fragmentPath) :
	vertex(vertexPath, GL_VERTEX_SHADER),
	fragment(fragmentPath, GL_FRAGMENT_SHADER)
{
	programID = glCreateProgram();

	vertex.attach(programID);
	fragment.attach(programID);
	glLinkProgram(programID);

	vertex.dealloc();
	fragment.dealloc();

	if (!checkLinkSuccess(programID)) {
		glDeleteProgram(programID);
		throw std::runtime_error("Shaders did not link.");
	}
}


ShaderProgram::ShaderProgram(ShaderProgram&& other) :
	programID(std::move(other.programID)),
	vertex(std::move(other.vertex)),
	fragment(std::move(other.fragment))
{
	other.programID = 0;
}


ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) {

	dealloc();

	programID = std::move(other.programID);
	vertex = std::move(other.vertex);
	fragment = std::move(other.fragment);

	other.programID = 0;

	return *this;
}


ShaderProgram::~ShaderProgram() {
	dealloc();
}


void ShaderProgram::dealloc() {
	glDeleteProgram(programID);
}


bool ShaderProgram::recompile() {

	try {
		Shader newVertex(vertex.getPath(), GL_VERTEX_SHADER);
		Shader newFragment(fragment.getPath(), GL_FRAGMENT_SHADER);

		GLuint newProgramID = glCreateProgram();

		newVertex.attach(newProgramID);
		newFragment.attach(newProgramID);
		glLinkProgram(newProgramID);

		newVertex.dealloc();
		newFragment.dealloc();

		if (checkLinkSuccess(newProgramID)) {

			vertex = std::move(newVertex);
			fragment = std::move(newFragment);

			glDeleteProgram(programID);
			programID = newProgramID;
			return true;
		}
		else {
			std::cerr << "INFO::SHADER_PROGRAM falling back to previous version of shaders" << std::endl;
			return false;
		}
	}
	catch (std::runtime_error e) {
		std::cerr << "INFO::SHADER_PROGRAM falling back to previous version of shaders" << std::endl;
		return false;
	}
}


void ShaderProgram::use() const {
	glUseProgram(programID);
}


bool ShaderProgram::checkLinkSuccess(GLuint ID) {

	GLint success;

	// check for link errors
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		GLint logLength;
		glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<char> log(logLength);
		glGetProgramInfoLog(ID, logLength, NULL, log.data());

		std::cerr << "ERROR::SHADER_PROGRAM linking " << vertex.getPath() << " + " << fragment.getPath() << ":" << std::endl;
		std::cerr << log.data() << std::endl;

		return false;
	}
	else {
		std::cerr << "INFO::SHADER_PROGRAM successfully compiled and linked " << vertex.getPath() << " + " << fragment.getPath() << std::endl;
		return true;
	}
}
