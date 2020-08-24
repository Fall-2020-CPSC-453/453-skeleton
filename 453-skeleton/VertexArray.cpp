#include "VertexArray.h"

#include <utility>


VertexArray::VertexArray() {
	glGenVertexArrays(1, &arrayID);
	glBindVertexArray(arrayID);
}


VertexArray::VertexArray(VertexArray&& other) noexcept
	: arrayID(std::move(other.arrayID))
{
	other.arrayID = 0;
}


VertexArray& VertexArray::operator=(VertexArray&& other) noexcept {

	this->~VertexArray();

	arrayID = std::move(other.arrayID);

	other.arrayID = 0;
	return *this;
}


VertexArray::~VertexArray() {
	glDeleteVertexArrays(1, &arrayID);
}
