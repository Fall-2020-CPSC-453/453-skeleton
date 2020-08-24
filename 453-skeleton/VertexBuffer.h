#pragma once

#include <GL/glew.h>

#include "GLHandles.h"


class VertexBuffer {

public:
	VertexBuffer(GLuint index, GLint size, GLenum dataType);

	// Public interface
	void bind() { glBindBuffer(GL_ARRAY_BUFFER, bufferID); }
	void uploadData(GLsizeiptr size, void* data, GLenum usage);

private:
	BufferID bufferID;
};

