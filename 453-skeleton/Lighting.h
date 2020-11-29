//------------------------------------------------------------------------------
// This file contains the lighting equation from Assignment4.
//------------------------------------------------------------------------------
#pragma once

#include <glm/glm.hpp>

#include "Scene.h"

// This object represents the "material" that is covering an object.
// Like its colour, reflection parameter and its specular parameters
struct ObjectMaterial {
	glm::vec3 color;
	float reflectionStrength = 0;
	float specularCoefficient = 0;
	glm::vec3 specular;
	glm::vec3 diffuse;
	float ambient;

	ObjectMaterial() : color(0, 0, 0) {}
};

struct FragmentShadingParameters {
	// Information about the point we're shading
	Intersection intersection;

	// The point's material parameters
	ObjectMaterial material;

	// Information about the ray being used.
	Ray ray;

	// Information about the scene
	Scene scene;

	// This must be set when an point has no path to the light
	bool inShadow; // Set this when the pixel is in shadow

	// These parameters must be set for objects that show reflections
	glm::vec3 reflectedColor; // The reflectedColor


	// Helper methods to name things the same as lecture
	glm::vec3 l() const { return intersection.lightPosition - p; }
	glm::vec3 n() const { return intersection.normal; }
	glm::vec3 p() const { return intersection.near; }
	glm::vec3 Ld() const { return scene.lightColor; }
	glm::vec3 Ls() const { return scene.lightColor; }
	glm::vec3 La() const { return scene.lightColor; }
	glm::vec3 Kd() const { return material.diffuse; }
	glm::vec3 Ks() const { return material.specular; }
	float Ka() const { return material.ambient; }
	float alpha() const { return material.specularCoefficient; }

};

// Calculate the ambient factor.
glm::vec3 ambient(FragmentShadingParameters params);

// Calculate the diffuse factor.
glm::vec3 diffuse(FragmentShadingParameters params);

// Calculate the specular factor.
float specular(FragmentShadingParameters params);

// Put it all together into a phone shaded equation
glm::vec3 phongShading(FragmentShadingParameters params);
