#include "VertexBuffer.h"

#include <utility>


VertexBuffer::VertexBuffer(GLuint index, GLint size, GLenum dataType)
	: bufferID{}
{
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glVertexAttribPointer(index, size, dataType, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(index);
}

void VertexBuffer::uploadData(GLsizeiptr size, void* data, GLenum usage) {
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glBufferData(GL_ARRAY_BUFFER, size, data, usage);
}
