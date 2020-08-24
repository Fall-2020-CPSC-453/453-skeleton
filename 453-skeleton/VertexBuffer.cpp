#include "VertexBuffer.h"

#include <utility>


VertexBuffer::VertexBuffer(GLuint index, GLint size, GLenum dataType) {
	glGenBuffers(1, &bufferID);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glVertexAttribPointer(index, size, dataType, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(index);
}


VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
	: bufferID(std::move(other.bufferID))
{
	other.bufferID = 0;
}


VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept {
	
	this->~VertexBuffer();

	bufferID = std::move(other.bufferID);

	other.bufferID = 0;
	return *this;
}


VertexBuffer::~VertexBuffer() {
	glDeleteBuffers(1, &bufferID);
}


void VertexBuffer::uploadData(GLsizeiptr size, void* data, GLenum usage) {
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glBufferData(GL_ARRAY_BUFFER, size, data, usage);
}
