#include "ShaderProgram.h"

#include <iostream>
#include <vector>

#include "Log.h"


ShaderProgram::ShaderProgram(std::string vertexPath, std::string fragmentPath)
	: programID()
	, vertex(vertexPath, GL_VERTEX_SHADER)
	, fragment(fragmentPath, GL_FRAGMENT_SHADER)
{
	attach(vertex);
	attach(fragment);
	glLinkProgram(programID);

	if (!checkLinkSuccess(programID)) {
		throw ShaderLinkException("Shaders did not link.");
	}
}


bool ShaderProgram::recompile() {
	try {
		// Try to create a new program
		ShaderProgram newProgram(vertex.getPath(), fragment.getPath());
		*this = std::move(newProgram);
		return true;
	}
	catch (ShaderCompileException) {
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
