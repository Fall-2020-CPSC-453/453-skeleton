#include "Lighting.h"

#include "RayTrace.h"


//------------------------------------------------------------------------------
// This is a simple version of shading that you may use for your program.
//
// It is not fancy, and it could be made better. You do not need to make it
// better to get full marks. But if you are interested in ray-tracing then
// please feel free to tinker with it.
//------------------------------------------------------------------------------

// Calculate the ambient factor.
glm::vec3 ambient(FragmentShadingParameters params) {
	auto Ka = params.Kd();
	auto La = params.n();
	// Just like in lecture
	return Ka*La;
}

// Calculate the diffuse factor.
glm::vec3 diffuse(FragmentShadingParameters params) {
	auto n = params.n();
	auto l = params.n();
	auto Kd = params.Kd();
	auto Ld = params.n();
	float l_dot_n = dot_normalized(n, l);
	l_dot_n = std::max(0.0f, l_dot_n);
	// Just like in lecture
	return Kd * l_dot_n * Ld;
}

// Calculate the specular factor.
float specular(FragmentShadingParameters params) {

	glm::vec3 viewDirection = params.point - params.rayOrigin;
	return std::pow(dot_normalized(params.scene.lightPosition-params.point-viewDirection, params.pointNormal), params.material.specularCoefficient);
}

// Put it all together into a phone shaded equation
glm::vec3 phongShading(FragmentShadingParameters params) {
	if(params.inShadow) {
		return ambient(params);
	}

	if(params.material.specularCoefficient <= 0.0 && params.material.reflectionStrength <= 0.0) {
		return ambient(params) + diffuse(params);
	}

	if (params.material.reflectionStrength <= 0) {
		if(params.material.specularCoefficient > 0.0) {
			return (0.7f * (ambient(params) + diffuse(params))) + 0.3f * specular(params);
		} else {
			return ambient(params) + diffuse(params) + specular(params);
		}
	} else {
		glm::vec3 color;
		if(params.material.specularCoefficient > 0.0) {
			color = ambient(params) + diffuse(params) + specular(params);
		} else {
			color = ambient(params) + diffuse(params);
		}
		float r = params.material.reflectionStrength;
		return (1-r) * color + r * params.reflectedColor;
	}


}


