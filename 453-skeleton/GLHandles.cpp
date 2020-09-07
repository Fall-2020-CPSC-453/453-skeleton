#include "GLHandles.h"

#include <algorithm> // For std::swap

ShaderHandle::ShaderHandle(GLenum type)
	: shaderID(glCreateShader(type))
{
}


ShaderHandle::ShaderHandle(ShaderHandle&& other) noexcept
	: shaderID(std::move(other.shaderID))
{
	other.shaderID = 0;
}


ShaderHandle& ShaderHandle::operator=(ShaderHandle&& other) noexcept {
	// Note that swap is implemented with move internally
	std::swap(shaderID, other.shaderID);
	return *this;
}


ShaderHandle::~ShaderHandle() {
	glDeleteShader(shaderID);
}


ShaderHandle::operator GLuint() const {
	return shaderID;
}

GLuint ShaderHandle::value() const {
	return shaderID;
}


//------------------------------------------------------------------------------


ShaderProgramHandle::ShaderProgramHandle()
	: programID(glCreateProgram())
{ }


ShaderProgramHandle::ShaderProgramHandle(ShaderProgramHandle&& other) noexcept
	: programID(std::move(other.programID))
{
	other.programID = 0;
}


ShaderProgramHandle& ShaderProgramHandle::operator=(ShaderProgramHandle&& other) noexcept {
	std::swap(programID, other.programID);
	return *this;
}


ShaderProgramHandle::~ShaderProgramHandle() {
	glDeleteProgram(programID);
}


ShaderProgramHandle::operator GLuint() const {
	return programID;
}


GLuint ShaderProgramHandle::value() const {
	return programID;
}

