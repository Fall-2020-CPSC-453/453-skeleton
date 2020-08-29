#include "ShaderProgram.h"

#include <iostream>
#include <stdexcept>
#include <vector>

#include "Log.h"


ShaderProgram::ShaderProgram(std::string vertexPath, std::string fragmentPath)
	: vertex(vertexPath, GL_VERTEX_SHADER)
	, fragment(fragmentPath, GL_FRAGMENT_SHADER)
{
	programID = glCreateProgram();

	attach(*this, vertex);
	attach(*this, fragment);
	glLinkProgram(programID);

	if (!checkLinkSuccess(programID)) {
		glDeleteProgram(programID);
		throw std::runtime_error("Shaders did not link.");
	}
}


ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept
	: programID(std::move(other.programID))
	, vertex(std::move(other.vertex))
	, fragment(std::move(other.fragment))
{
	other.programID = 0;
}


ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) noexcept {

	this->~ShaderProgram();

	programID = std::move(other.programID);
	vertex = std::move(other.vertex);
	fragment = std::move(other.fragment);

	other.programID = 0;
	return *this;
}


ShaderProgram::~ShaderProgram() {
	glDeleteProgram(programID);
}


bool ShaderProgram::recompile() {

	try {
		// Try to create a new program
		ShaderProgram newProgram(vertex.getPath(), fragment.getPath());
		*this = std::move(newProgram);
		return true;
	}
	catch (std::runtime_error &e) {
		Log::warn("SHADER_PROGRAM falling back to previous version of shaders");
		return false;
	}
}


void ShaderProgram::use() const {
	glUseProgram(programID);
}


void attach(ShaderProgram& sp, Shader& s) {
	glAttachShader(sp.programID, s.shaderID);
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

		Log::error("SHADER_PROGRAM linking {} + {}:\n{}", vertex.getPath(), fragment.getPath(), log.data());

		return false;
	}
	else {
		Log::info("SHADER_PROGRAM successfully compiled and linked {} + {}", vertex.getPath(), fragment.getPath());
		return true;
	}
}
