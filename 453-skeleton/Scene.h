//------------------------------------------------------------------------------
// A basic scene class and description
//
// You don't need to change this for the base assignment, you may need to change
// this for bonus.
//------------------------------------------------------------------------------
#pragma once

#include "RayTrace.h"
#include <memory>

class Shape;

struct Scene {
	glm::vec3 light;
	glm::vec3 light_color;
	float ambient;
	float diffuse;
	std::vector<std::shared_ptr<Shape>> shapesInScene;
};


Scene initScene1();
Scene initScene2();
Scene initScene3();

