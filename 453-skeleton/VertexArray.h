#pragma once

#include <GL/glew.h>


class VertexArray {

public:
	VertexArray();

	// Copying not allowed
	VertexArray(const VertexArray&) = delete;
	VertexArray operator=(const VertexArray&) = delete;

	// Moving is allowed
	VertexArray(VertexArray&& other) noexcept;
	VertexArray& operator=(VertexArray&& other) noexcept;

	// Destructor to cleanup resources on GPU
	~VertexArray();


	// Public interface
	void bind() { glBindVertexArray(arrayID); }

private:
	GLuint arrayID;
};
