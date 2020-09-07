#include "GLHandles.h"

#include "Log.h"

#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>


ShaderHandle::ShaderHandle(GLenum type)
	: shaderID{glCreateShader(type)}
{
}


ShaderHandle::ShaderHandle(ShaderHandle&& other) noexcept
	: shaderID(std::move(other.shaderID))
{
	other.shaderID = 0;
}


ShaderHandle& ShaderHandle::operator=(ShaderHandle&& other) noexcept {
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
