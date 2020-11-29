#include "Scene.h"

#include <cmath>

#include <algorithm> // For std::max
#include <limits> // For std::numeric_limits

// Some constants defining the various scenes

//Reflective grey sphere
vec3 centre = vec3( 0.9, -1.925, -6.69);
float radius = 0.825;


//Blue pyramid
vec3 blue_pyramid[12]{
  vec3(-0.4, -2.75, -9.55),
  vec3(-0.93, 0.55, -8.51),
  vec3(0.11, -2.75, -7.98),

  vec3(0.11, -2.75, -7.98),
  vec3(-0.93, 0.55, -8.51),
  vec3(-1.46, -2.75, -7.47),

  vec3(-1.46, -2.75, -7.47),
  vec3(-0.93, 0.55, -8.51),
  vec3(-1.97, -2.75, -9.04),

  vec3(-1.97, -2.75, -9.04),
  vec3(-0.93, 0.55, -8.51),
  vec3(-0.4, -2.75, -9.55)
};




//Floor
vec3 floor1[6]{
  vec3(2.75, -2.75, -5),
  vec3(2.75, -2.75, -10.5),
  vec3(-2.75, -2.75, -10.5),
  vec3(-2.75, -2.75, -5),
  vec3(2.75, -2.75, -5),
  vec3(-2.75, -2.75, -10.5)
};
//Ceiling
vec3 ceiling[6]{
  vec3(2.75, 2.75, -10.5),
  vec3(2.75, 2.75, -5),
  vec3(-2.75, 2.75, -5),

  vec3(-2.75, 2.75, -10.5),
  vec3(2.75, 2.75, -10.5),
  vec3(-2.75, 2.75, -5)
};

//Green wall on right
vec3 right_wall[6]{
  vec3(2.75, 2.75, -5),
  vec3(2.75, 2.75, -10.5),
  vec3(2.75, -2.75, -10.5),

  vec3(2.75, -2.75, -5),
  vec3(2.75, 2.75, -5),
  vec3(2.75, -2.75, -10.5)
};

//Red wall on left
vec3 left_wall[6]{
  vec3(-2.75, -2.75, -5),
  vec3(-2.75, -2.75, -10.5),
  vec3(-2.75, 2.75, -10.5),

  vec3(-2.75, 2.75, -5),
  vec3(-2.75, -2.75, -5),
  vec3(-2.75, 2.75, -10.5)
};

////Back wall
//vec3 back_wall[2] {
//  vec3(0, 0, 1),	//normal
//  vec3(0, 0, -10.5)	//point
//};

//Back wall
vec3 back_wall[6] {
	vec3(2.75, -2.75, -10.5),
	vec3(2.75, 2.75, -10.5),
	vec3(-2.75, 2.75, -10.5),

	vec3(-2.75, 2.75, -10.5),
	vec3(-2.75, -2.75, -10.5),
	vec3(2.75, -2.75, -10.5)
};

//---------------------------------------------------------------------------
// Shiny red icosahedron
vec3 icosahedron[] {
	vec3(-2, -1, -7),
	vec3(-1.276, -0.4472, -6.474),
	vec3(-2.276, -0.4472, -6.149),

	vec3(-1.276, -0.4472, -6.474),
	vec3(-2 ,-1, -7),
	vec3(-1.276 ,-0.4472 ,-7.526),

	vec3(-2, -1 ,-7),
	vec3(-2.276 ,-0.4472 ,-6.149),
	vec3(-2.894 ,-0.4472, -7),

	vec3(-2 ,-1 ,-7),
	vec3(-2.894 ,-0.4472 ,-7),
	vec3(-2.276 ,-0.4472, -7.851),

	vec3(-2 ,-1 ,-7),
	vec3(-2.276 ,-0.4472 ,-7.851),
	vec3(-1.276 ,-0.4472 ,-7.526),

	vec3(-1.276 ,-0.4472, -6.474),
	vec3(-1.276 ,-0.4472 ,-7.526),
	vec3(-1.106, 0.4472, -7),

	vec3(-2.276, -0.4472 ,-6.149),
	vec3(-1.276, -0.4472 ,-6.474),
	vec3(-1.724 ,0.4472, -6.149),

	vec3(-2.894 ,-0.4472, -7),
	vec3(-2.276, -0.4472, -6.149),
	vec3(-2.724 ,0.4472 ,-6.474),

	vec3(-2.276, -0.4472, -7.851),
	vec3(-2.894 ,-0.4472 ,-7),
	vec3( -2.724 ,0.4472 ,-7.526),

	vec3(-1.276, -0.4472, -7.526),
	vec3(-2.276, -0.4472 ,-7.851),
	vec3(-1.724, 0.4472, -7.851),

	vec3(-1.276 ,-0.4472 ,-6.474),
	vec3(-1.106, 0.4472, -7),
	vec3(-1.724 ,0.4472, -6.149),

	vec3( -2.276 ,-0.4472 ,-6.149),
	vec3(-1.724, 0.4472, -6.149),
	vec3(-2.724 ,0.4472, -6.474),

	vec3( -2.894, -0.4472, -7),
	vec3( -2.724, 0.4472, -6.474),
	vec3( -2.724, 0.4472, -7.526),

	vec3(-2.276 ,-0.4472 ,-7.851),
	vec3( -2.724 ,0.4472 ,-7.526),
	vec3( -1.724 ,0.4472 ,-7.851),

	vec3(-1.276 ,-0.4472 ,-7.526),
	vec3( -1.724 ,0.4472, -7.851),
	vec3( -1.106, 0.4472, -7),

	vec3(-1.724, 0.4472, -6.149),
	vec3(-1.106, 0.4472 ,-7),
	vec3( -2, 1, -7),

	vec3( -2.724 ,0.4472 ,-6.474),
	vec3(-1.724, 0.4472 ,-6.149),
	vec3(-2, 1, -7),

	vec3(-2.724, 0.4472, -7.526),
	vec3( -2.724, 0.4472 ,-6.474),
	vec3(-2, 1 ,-7),

	vec3(-1.724 ,0.4472 ,-7.851),
	vec3(-2.724, 0.4472 ,-7.526),
	vec3(-2 ,1 ,-7),

	vec3(-1.106 ,0.4472 ,-7),
	vec3(-1.724 ,0.4472 ,-7.851),
	vec3(-2, 1, -7)
};

// Green cone
vec3 green_Cone[] {
	vec3(0, -1, -5.8),
	vec3(0 ,0.6, -5),
	vec3(0.4, -1 ,-5.693),

	vec3(0.4 ,-1, -5.693),
	vec3(0, 0.6, -5),
	vec3(0.6928 ,-1 ,-5.4),

	vec3(0.6928 ,-1 ,-5.4),
	vec3(0 ,0.6 ,-5),
	vec3(0.8 ,-1 ,-5),

	vec3(0.8 ,-1 ,-5),
	vec3(0,0.6 ,-5),
	vec3(0.6928, -1 ,-4.6),

	vec3( 0.6928, -1 ,-4.6),
	vec3(0 ,0.6, -5),
	vec3(0.4, -1 ,-4.307),

	vec3(0.4 ,-1 ,-4.307),
	vec3(0 ,0.6, -5),
	vec3(0 ,-1 ,-4.2),

	vec3(0 ,-1 ,-4.2),
	vec3(0, 0.6, -5),
	vec3(-0.4, -1 ,-4.307),

	vec3(-0.4 ,-1, -4.307),
	vec3(0 ,0.6 ,-5),
	vec3(-0.6928 ,-1 ,-4.6),

	vec3(-0.6928, -1 ,-4.6),
	vec3(0, 0.6 ,-5),
	vec3(-0.8 ,-1 ,-5),

	vec3(-0.8 ,-1 ,-5),
	vec3(0 ,0.6 ,-5),
	vec3(-0.6928 ,-1, -5.4),

	vec3(-0.6928 ,-1 ,-5.4),
	vec3(0, 0.6, -5),
	vec3(-0.4, -1, -5.693),

	vec3(-0.4 ,-1, -5.693),
	vec3(0, 0.6 ,-5),
	vec3(0 ,-1 ,-5.8)
};

//Floor
vec3 floor2[]{
		vec3(-10,-1,-2),
		vec3(10,-1,-2),
		vec3(10,-1,-12),

		vec3(10,-1,-12),
		vec3(-10,-1,-12),
		vec3(-10,-1,-2)
};

//Back wall
vec3 back_wall2[]{
		vec3(-10, -1, -12),
		vec3(10, -1, -12),
		vec3(10, 9, -12),

		vec3(10,9,-12),
		vec3(-10, 9,-12),
		vec3(-10, -1, -12)
};

Scene initScene1() {
	//Scene 1
	Scene scene1;

	//Reflective grey sphere
	std::shared_ptr<Sphere> sphere1 = std::make_shared<Sphere>(
		vec3( 0.9, -1.925, -6.69), 0.825, 1
	);
	sphere1->material.diffuse = vec3(0.6,0.6,0.6);
	sphere1->material.reflectionStrength = vec3(0.4);
	//pyramid
	std::shared_ptr<Triangles> pyramid = std::make_shared<Triangles>();
	std::shared_ptr<Triangles> rightWall = std::make_shared<Triangles>();
	std::shared_ptr<Triangles> leftWall = std::make_shared<Triangles>();
	std::shared_ptr<Triangles> floorWall = std::make_shared<Triangles>();
	std::shared_ptr<Triangles> ceilingWall = std::make_shared<Triangles>();
	std::shared_ptr<Triangles> backWall = std::make_shared<Triangles>();

	scene1.lightPosition = vec3(0,2.5,-7.75);
	scene1.lightColor = vec3(1,1,1);

	sphere1->material.specularCoefficient = 64;
	scene1.shapesInScene.push_back(sphere1);

	pyramid->initTriangles(4, blue_pyramid, 2);
	pyramid->material.diffuse = vec3(0.0,0.85,0.95);
	pyramid->material.reflectionStrength = vec3(0.3);
	scene1.shapesInScene.push_back(pyramid);

	rightWall->initTriangles(2, right_wall, 3);
	rightWall->material.diffuse = vec3(0.0, 0.7, 0.0);
	scene1.shapesInScene.push_back(rightWall);

	leftWall->initTriangles(2, left_wall, 4);
	leftWall->material.diffuse = vec3(0.7, 0.0, 0.0);
	scene1.shapesInScene.push_back(leftWall);

	floorWall->initTriangles(2, floor1, 5);
	floorWall->material.diffuse = vec3(0.8, 0.8, 0.8);
	scene1.shapesInScene.push_back(floorWall);

	ceilingWall->initTriangles(2, ceiling, 6);
	ceilingWall->material.diffuse = vec3(1.0, 1.0, 1.0);
	scene1.shapesInScene.push_back(ceilingWall);

	backWall->initTriangles(2, back_wall, 7);
	backWall->material.diffuse = vec3(1.0, 1.0, 1.0);
	scene1.shapesInScene.push_back(backWall);
	return scene1;
}

Scene initScene2() {
	//Scene 2
	Scene scene2;
	std::shared_ptr<Triangles> Icosahedron = std::make_shared<Triangles>();
	Icosahedron->initTriangles(20,icosahedron, 1);
	Icosahedron->material.diffuse = vec3(1.0, 0.0, 0.0);
	Icosahedron->material.reflectionStrength = vec3(0.5);
	scene2.shapesInScene.push_back(Icosahedron);

	// Large yellow sphere
	std::shared_ptr<Sphere> sphere2 = std::make_shared<Sphere>(
			vec3( 1, -0.5, -3.5), 0.5, 2);
	sphere2->material.diffuse = vec3(1.0, 1.0, 0.0);
	sphere2->material.specularCoefficient = 64;
	scene2.shapesInScene.push_back(sphere2);

	// Reflective grey sphere
	std::shared_ptr<Sphere> sphere3 = std::make_shared<Sphere>(
			vec3( 0, 1, -5), 0.4, 3);
	sphere3->material.diffuse = vec3(0.6, 0.6, 0.6);
	sphere3->material.reflectionStrength = vec3(0.5, 0.5, 0.5);
	sphere3->material.specularCoefficient = 64;
	scene2.shapesInScene.push_back(sphere3);


	// Metallic purple sphere
	std::shared_ptr<Sphere> sphere4 = std::make_shared<Sphere>(
			vec3( -0.8, -0.75, -4), 0.25, 4);
	sphere4->material.diffuse = vec3(0.4, 0.1, 1.0);
	sphere4->material.reflectionStrength = vec3(0.3);
	sphere4->material.specularCoefficient = 64;
	scene2.shapesInScene.push_back(sphere4);

	//Green cone
	std::shared_ptr<Triangles> greenCone = std::make_shared<Triangles>();
	greenCone->initTriangles(12,green_Cone, 5);
	greenCone->material.diffuse = vec3(0.0, 0.8, 0.0);
	greenCone->material.specularCoefficient = 8;
	scene2.shapesInScene.push_back(greenCone);

	//Floor
	std::shared_ptr<Plane> floorTwo = std::make_shared<Plane>(
			vec3(0,-1,0), vec3(0,1,0), 6);
	floorTwo->material.diffuse = vec3(0.8, 0.8, 0.8);
	scene2.shapesInScene.push_back(floorTwo);

	//Back wall
	std::shared_ptr<Plane> backWallTwo = std::make_shared<Plane>(
			vec3(0,0,-12), vec3(0,0,1), 7);
	backWallTwo->material.diffuse = vec3(0.0, 0.6, 0.6);
	backWallTwo->material.specularCoefficient = 8;
	scene2.shapesInScene.push_back(backWallTwo);

	scene2.lightPosition = vec3(4, 6, -1);
	scene2.lightColor = vec3(1,1,1);

	return scene2;
}


//Scene 3
vec3 diamondVerts[]{
	vec3(0.6, -1, -2.5),
	vec3(0.9, -0.3, -2.5),
	vec3(0.6, -0.3, -2.2),

	vec3(0.6, -1, -2.5),
	vec3(0.6, -0.3, -2.8),
	vec3(0.9, -0.3, -2.5),

	vec3(0.6, -1, -2.5),
	vec3(0.3, -0.3, -2.5),
	vec3(0.6, -0.3, -2.8),

	vec3(0.6, -1, -2.5),
	vec3(0.6, -0.3, -2.2),
	vec3(0.3, -0.3, -2.5)
};

Scene initScene3() {
	Scene scene3;

	std::shared_ptr<Plane> floor3 = std::make_shared<Plane>(
			vec3(0,-1,0), vec3(0,1,0), 1);
	floor3->material.diffuse = vec3(0.8, 0.8, 0.8);
	scene3.shapesInScene.push_back(floor3);

	std::shared_ptr<Plane> backWall3 = std::make_shared<Plane>(
			vec3(0,0,-12), vec3(0,0,1), 2);
	backWall3->material.diffuse = vec3(0.6, 0.7, 1.0);
	backWall3->material.specularCoefficient = 8;
	scene3.shapesInScene.push_back(backWall3);

	std::shared_ptr<Plane> leftWall3 = std::make_shared<Plane>(
			vec3(-2,0,0), vec3(1,0,0), 3);
	leftWall3->material.diffuse = vec3(0.8, 0.6, 0.9);
	leftWall3->material.reflectionStrength = glm::vec3(0.2);
	scene3.shapesInScene.push_back(leftWall3);

	std::shared_ptr<Sphere> sphere5 = std::make_shared<Sphere>(
			vec3( -1, -0.25, -6), 0.75, 4);
	sphere5->material.diffuse = vec3(0.4, 0.1, 1.0);
	sphere5->material.reflectionStrength = vec3(0.3);
	sphere5->material.specularCoefficient = 16;
	scene3.shapesInScene.push_back(sphere5);

	std::shared_ptr<Sphere> sphere6 = std::make_shared<Sphere>(
			vec3( 0.6, 0.4, -2.5), 0.4, 5);
	sphere6->material.diffuse = vec3(0.2, 0.5, 0.9);
	sphere6->material.reflectionStrength = vec3(0.5);
	sphere6->material.specularCoefficient = 8;
	scene3.shapesInScene.push_back(sphere6);

	std::shared_ptr<Sphere> sphere7 = std::make_shared<Sphere>(
			vec3( -0.5, -0.75, -4.5), 0.25, 6);
	sphere7->material.diffuse = vec3(0.4, 0.6, 0.8);
	sphere7->material.specularCoefficient = 64;
	scene3.shapesInScene.push_back(sphere7);

	scene3.lightPosition = vec3(4,6,-1);
	scene3.lightColor = vec3(1,1,1);

	std::shared_ptr<Triangles> diamond = std::make_shared<Triangles>();
	diamond->initTriangles(4, diamondVerts, 7);
	diamond->material.diffuse = vec3(0.0, 0.85, 0.95);
	diamond->material.reflectionStrength = vec3(0.3);
	diamond->material.specularCoefficient = 8;
	scene3.shapesInScene.push_back(diamond);
	return scene3;
}
