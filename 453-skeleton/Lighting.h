//------------------------------------------------------------------------------
// This file contains the lighting equation from Assignment4.
//------------------------------------------------------------------------------
#pragma once

#include <glm/glm.hpp>

#include "Scene.h"
#include "Material.h"

struct PhongReflection {
	// Information about the point we're shading
	Intersection intersection;

	// The point's material parameters
	ObjectMaterial material;

	// Information about the ray being used.
	Ray ray;

	// Information about the scene
	Scene scene;


	// Helper methods to name things the same as lecture
	glm::vec3 l() const { return glm::normalize(scene.lightPosition - p()); } // light vector
	glm::vec3 n() const { return glm::normalize(intersection.normal); } // normal
	glm::vec3 p() const { return intersection.point; } // point
	glm::vec3 v() const { return glm::normalize(ray.origin - p()); } // view direction
	glm::vec3 r() const { return -glm::reflect(l(), n()); } // reflected light vector

	glm::vec3 La() const { return scene.ambientFactor*scene.lightColor; } // Light ambient
	glm::vec3 Ld() const { return scene.lightColor; } // Light diffuse
	glm::vec3 Ls() const { return scene.lightColor; } // Light specular

	glm::vec3 Ka() const { return material.ambient; } // Material ambient
	glm::vec3 Kd() const { return material.diffuse; } // Material diffuse
	glm::vec3 Ks() const { return material.specular; } // Material specular

	float alpha() const { return material.specularCoefficient; }

	// Calculate the ambient factor.
	glm::vec3 Ia() const {
		// Just like in lecture.
		// NOTE: the following is component wise multiplication, NOT the dot product.
		return Ka()*La();
	}


	// Calculate the diffuse factor.
	glm::vec3 Id() const {
		// Calculate cos(theta)
		float l_dot_n = glm::dot(l(), n());
		// Ensure we don't get negative numbers
		l_dot_n = std::max(0.0f, l_dot_n);
		// combine it just like in lecture
		// NOTE: the following is component wise multiplication, NOT the dot product.
		return Kd() * l_dot_n * Ld();
	}

	// Calculate the specular factor.
	glm::vec3 Is() const {
		auto r_dot_v = glm::dot(r(), v());
		// Ensure we don't get negative numbers
		r_dot_v = std::max(0.0f, r_dot_v);
		r_dot_v = std::pow(r_dot_v, alpha());

		// NOTE: the following is component wise multiplication, NOT the dot product.
		return Ks() * Ls() * r_dot_v;
	}

	// Put it all together into a phong reflection equation
	glm::vec3 I() const {
		return Id() + Is() + Ia();
	}
};

