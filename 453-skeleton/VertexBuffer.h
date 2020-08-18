#pragma once

#include <GL/glew.h>


class VertexBuffer {

public:
	VertexBuffer(GLuint index, GLint size, GLenum dataType);

	// Copying not allowed
	VertexBuffer(const VertexBuffer&) = delete;
	VertexBuffer operator=(const VertexBuffer&) = delete;

	// Moving is allowed
	VertexBuffer(VertexBuffer&& other);
	VertexBuffer& operator=(VertexBuffer&& other);

	// Destructor to cleanup resources on GPU
	~VertexBuffer();
	void dealloc();


	// Public interface
	void bind() { glBindBuffer(GL_ARRAY_BUFFER, bufferID); }
	void uploadData(GLsizeiptr size, void* data, GLenum usage);

private:
	GLuint bufferID;
};

