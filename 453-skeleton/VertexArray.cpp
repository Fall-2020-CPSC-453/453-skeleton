#include "VertexArray.h"

#include <utility>


VertexArray::VertexArray() {
	glGenVertexArrays(1, &arrayID);
	glBindVertexArray(arrayID);
}


VertexArray::VertexArray(VertexArray&& other) :
	arrayID(std::move(other.arrayID))
{
	other.arrayID = 0;
}


VertexArray& VertexArray::operator=(VertexArray&& other) {

	dealloc();

	arrayID = std::move(other.arrayID);

	other.arrayID = 0;
	return *this;
}


VertexArray::~VertexArray() {
	dealloc();
}


void VertexArray::dealloc() {
	glDeleteVertexArrays(1, &arrayID);
}