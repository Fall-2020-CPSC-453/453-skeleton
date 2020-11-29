#include "Material.h"

ObjectMaterial goldFromSomeRandomWebsite() {
	// https://en.wikibooks.org/wiki/Blender_3D:_Noob_to_Pro/Every_Material_Known_to_Man/Gold
	ObjectMaterial mat;
	mat.diffuse = glm::vec3(0.912, 0.782, 0.082);
	mat.ambient = 0.1f * mat.diffuse;//glm::vec3(0.33, 0.22, 0.03);
	mat.specular = glm::vec3(1.00, 0.913, 0.8);
	mat.reflectionStrength = 0.1f*glm::vec3(0.857, 0.90, 0.70);
	mat.specularCoefficient = 27.8f;

	return mat;
}

ObjectMaterial brassFromLecture() {
	ObjectMaterial mat;
	mat.ambient = glm::vec3(0.33, 0.22, 0.03);
	mat.diffuse = glm::vec3(0.78, 0.57, 0.11);
	mat.specular = glm::vec3(0.99, 0.91, 0.81);
	mat.specularCoefficient = 27.8f;
	mat.reflectionStrength = glm::vec3(0.1);
	return mat;
}
