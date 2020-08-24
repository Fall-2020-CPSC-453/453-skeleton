#pragma once

#include <GL/glew.h>

#include "GLHandles.h"


class VertexArray {

public:
	VertexArray();

	// Public interface
	void bind() { glBindVertexArray(arrayID); }

private:
	VertexArrayID arrayID;
};
