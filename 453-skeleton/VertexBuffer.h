#pragma once

#include <GL/glew.h>

#include "GLHandles.h"


class VertexBuffer {

public:
	VertexBuffer(GLuint index, GLint size, GLenum dataType);

	// Copying not allowed
	VertexBuffer(const VertexBuffer&) = delete;
	VertexBuffer operator=(const VertexBuffer&) = delete;

	// Moving is allowed
	VertexBuffer(VertexBuffer&& other) = default;
	VertexBuffer& operator=(VertexBuffer&& other) = default;

	// Public interface
	void bind() { glBindBuffer(GL_ARRAY_BUFFER, bufferID); }
	void uploadData(GLsizeiptr size, void* data, GLenum usage);

private:
	BufferID bufferID;
};

