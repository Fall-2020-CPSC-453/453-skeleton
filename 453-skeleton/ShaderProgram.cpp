#include "ShaderProgram.h"

#include <iostream>
#include <stdexcept>
#include <vector>

#include "Log.h"


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
			Log::warn("SHADER_PROGRAM falling back to previous version of shaders");
			return false;
		}
	}
	catch (std::runtime_error &e) {
		Log::warn("SHADER_PROGRAM falling back to previous version of shaders");
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
		std::vector<char> log((size_t)logLength);
		glGetProgramInfoLog(ID, logLength, NULL, log.data());

		Log::error("SHADER_PROGRAM linking {} + {}:\n{}", vertex.getPath(), fragment.getPath(), log.data());

		return false;
	}
	else {
		Log::info("SHADER_PROGRAM successfully compiled and linked {} + {}", vertex.getPath(), fragment.getPath());
		return true;
	}
}
