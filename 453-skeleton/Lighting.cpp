#include "Lighting.h"

#include "Lighting.h"

// Calculate the ambient factor.
glm::vec3 ambient(FragmentShadingParameters params) {
	return params.sceneAmbient * params.pointColor;
}

// Calculate the diffuse factor.
glm::vec3 diffuse(FragmentShadingParameters params) {
	float tmpDiffuse = std::max(0.0f, dot_normalized(params.pointNormal, params.lightPosition-params.point));
	return params.sceneDiffuse * tmpDiffuse * params.pointColor;
}

// Calculate the specular factor.
float specular(FragmentShadingParameters params) {
	glm::vec3 viewDirection = params.point - params.eyePosition;
	return std::pow(dot_normalized(params.lightPosition-params.point-viewDirection, params.pointNormal), params.pointSpecular);
}

// Put it all together into a phone shaded equation
glm::vec3 phongShading(FragmentShadingParameters params) {
	if(params.inShadow) {
		return params.sceneAmbient * params.pointColor;
	}

	if(params.pointSpecular <= 0.0) {
		return ambient(params) + diffuse(params);
	}

	if(params.reflectionStrength <= 0 && params.pointSpecular > 0.0) {
		return (0.7f * (ambient(params) + diffuse(params))) + 0.3f * specular(params);
	}

	return ambient(params) + diffuse(params) + specular(params);

}


