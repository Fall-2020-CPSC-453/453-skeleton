#pragma once

#include <GL/glew.h>


// An RAII class for managing a Shader GLuint for OpenGL
class ShaderHandle {

public:
	ShaderHandle(GLenum type);

	// Copying not allowed
	ShaderHandle(const ShaderHandle&) = delete;
	ShaderHandle operator=(const ShaderHandle&) = delete;

	// Moving is allowed
	ShaderHandle(ShaderHandle&& other) noexcept;
	ShaderHandle& operator=(ShaderHandle&& other) noexcept;

	// Destructor to cleanup resources on GPU
	~ShaderHandle();


	// Allow casting from this type into a GLuint
	// This allows usage in situations where a function expects a GLuint
	operator GLuint() const;
	GLuint value() const;

private:
	GLuint shaderID;

};


