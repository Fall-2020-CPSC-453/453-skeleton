#pragma once

#include <GL/glew.h>

#include "GLHandles.h"


class VertexArray {

public:
	VertexArray();

	// Copying not allowed
	VertexArray(const VertexArray&) = delete;
	VertexArray operator=(const VertexArray&) = delete;

	// Moving is allowed
	VertexArray(VertexArray&& other) = default;
	VertexArray& operator=(VertexArray&& other) = default;


	// Public interface
	void bind() { glBindVertexArray(arrayID); }

private:
	VertexArrayID arrayID;
};
