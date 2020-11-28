//------------------------------------------------------------------------------
// This file contains the lighting equation from Assignment4.
//------------------------------------------------------------------------------
#pragma once

#include <glm/glm.hpp>
#include "RayTrace.h"

struct FragmentShadingParameters {
	// Information about the point we're shading
	glm::vec3 point; // The point we are shading in the scene.
	glm::vec3 pointNormal; // The normal of the point we are shading in the scene.
	glm::vec3 pointColor; // The color of the point we are shading
	float pointSpecular; // The specular factor of the object

	// Information about the camera and light.
	glm::vec3 rayOrigin;
	glm::vec3 lightPosition; // The position of the light


	float sceneAmbient; // The amount of ambient in the scene
	float sceneDiffuse; // The amount of diffuse in the scene


	// This must be set when an point has no path to the light
	bool inShadow; // Set this when the pixel is in shadow


	// These parameters must be set for objects that show reflections
	float reflectionStrength; // The strength of the reflection
	glm::vec3 reflectedColor; // The reflectedColor
};

// Calculate the ambient factor.
glm::vec3 ambient(FragmentShadingParameters params);

// Calculate the diffuse factor.
glm::vec3 diffuse(FragmentShadingParameters params);

// Calculate the specular factor.
float specular(FragmentShadingParameters params);

// Put it all together into a phone shaded equation
glm::vec3 phongShading(FragmentShadingParameters params);
