//------------------------------------------------------------------------------
// This file contains the lighting equation from Assignment4.
//------------------------------------------------------------------------------
#pragma once

#include <glm/glm.hpp>

// This object represents the "material" that is covering an object.
// Like its colour, reflection parameter and its specular parameters
struct ObjectMaterial {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 reflectionStrength;
	float specularCoefficient = 0;

	ObjectMaterial()
		: ambient(0.0, 0.0, 0.0)
		, diffuse(0, 0, 0)
		, specular(0, 0, 0)
		, reflectionStrength(0, 0, 0)
	{}
};

ObjectMaterial goldFromSomeRandomWebsite();
ObjectMaterial brassFromLecture();
