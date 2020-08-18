#pragma once

#include "VertexArray.h"
#include "VertexBuffer.h"

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <vector>


struct CPU_Geometry {
	std::vector<glm::vec3> verts;
	std::vector<glm::vec3> cols;
};


class GPU_Geometry {

public:
	GPU_Geometry();

	// Public interface
	void bind() { vao.bind(); }

	void setVerts(std::vector<glm::vec3>& verts);
	void setCols(std::vector<glm::vec3>& cols);

private:
	// note: due to how OpenGL works, vao needs to be 
	// defined and initialized before the vertex buffers
	VertexArray vao;

	VertexBuffer vertBuffer;
	VertexBuffer colBuffer;
};