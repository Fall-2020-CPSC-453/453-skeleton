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
  vec3(2.75, -2.75, -2),
  vec3(2.75, -2.75, -10.5),
  vec3(-2.75, -2.75, -10.5),
  vec3(-2.75, -2.75, -2),
  vec3(2.75, -2.75, -2),
  vec3(-2.75, -2.75, -10.5)
};
//Ceiling
vec3 ceiling[6]{
  vec3(2.75, 2.75, -10.5),
  vec3(2.75, 2.75, -2),
  vec3(-2.75, 2.75, -2),

  vec3(-2.75, 2.75, -10.5),
  vec3(2.75, 2.75, -10.5),
  vec3(-2.75, 2.75, -2)
};

//Green wall on right
vec3 right_wall[6]{
  vec3(2.75, 2.75, -2),
  vec3(2.75, 2.75, -10.5),
  vec3(2.75, -2.75, -10.5),

  vec3(2.75, -2.75, -2),
  vec3(2.75, 2.75, -2),
  vec3(2.75, -2.75, -10.5)
};

//Red wall on left
vec3 left_wall[6]{
  vec3(-2.75, -2.75, -2),
  vec3(-2.75, -2.75, -10.5),
  vec3(-2.75, 2.75, -10.5),

  vec3(-2.75, 2.75, -2),
  vec3(-2.75, -2.75, -2),
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
		vec3( 0.9, -1.925, -6.69), 0.825, vec3(0.6,0.6,0.6), 0.5, 1
	);
	//pyramid
	std::shared_ptr<Triangles> pyramid = std::make_shared<Triangles>();
	std::shared_ptr<Triangles> rightWall = std::make_shared<Triangles>();
	std::shared_ptr<Triangles> leftWall = std::make_shared<Triangles>();
	std::shared_ptr<Triangles> floorWall = std::make_shared<Triangles>();
	std::shared_ptr<Triangles> ceilingWall = std::make_shared<Triangles>();
	std::shared_ptr<Triangles> backWall = std::make_shared<Triangles>();

	scene1.light = vec3(0,2.5,-7.75);
	scene1.ambient = 0.1;
	scene1.diffuse = 0.4;
	scene1.light_color = vec3(1,1,1);

	sphere1->material.specularStrength = 64;
	scene1.shapesInScene.push_back(sphere1);

	pyramid->initTriangles(4, blue_pyramid, vec3(0,0.85,0.95), 0.3, 2);
	scene1.shapesInScene.push_back(pyramid);

	rightWall->initTriangles(2,right_wall, vec3(0,0.7,0), 0, 3);
	scene1.shapesInScene.push_back(rightWall);

	leftWall->initTriangles(2,left_wall, vec3(0.7,0,0), 0, 4);
	scene1.shapesInScene.push_back(leftWall);

	floorWall->initTriangles(2,floor1, vec3(0.8,0.8,0.8), 0, 5);
	scene1.shapesInScene.push_back(floorWall);

	ceilingWall->initTriangles(2,ceiling, vec3(1,1,1), 0, 6);
	scene1.shapesInScene.push_back(ceilingWall);

	backWall->initTriangles(2,back_wall, vec3(1,1,1), 0, 7);
	scene1.shapesInScene.push_back(backWall);
	return scene1;
}

Scene initScene2() {
	//Scene 2
	Scene scene2;
	std::shared_ptr<Triangles> Icosahedron = std::make_shared<Triangles>();
	// Large yellow sphere
	std::shared_ptr<Sphere> sphere2 = std::make_shared<Sphere>(
			vec3( 1, -0.5, -3.5), 0.5, vec3(1,1,0), 0, 2);

	// Reflective grey sphere
	std::shared_ptr<Sphere> sphere3 = std::make_shared<Sphere>(
			vec3( 0, 1, -5), 0.4, vec3(0.6,0.6,0.6), 0.5, 3);
	// Metallic purple sphere
	std::shared_ptr<Sphere> sphere4 = std::make_shared<Sphere>(
			vec3( -0.8, -0.75, -4), 0.25, vec3(0.4,0.1,1), 0.3, 4);
	//Green cone
	std::shared_ptr<Triangles> greenCone = std::make_shared<Triangles>();
	//Floor
	std::shared_ptr<Plane> floorTwo = std::make_shared<Plane>(
			vec3(0,-1,0), vec3(0,1,0),vec3(0.8,0.8,0.8), 0, 6);
	//Back wall
	std::shared_ptr<Plane> backWallTwo = std::make_shared<Plane>(
			vec3(0,0,-12), vec3(0,0,1), vec3(0,0.6,0.6), 0,7);

	scene2.light = vec3(4,6,-1);
	scene2.ambient = 0.1;
	scene2.diffuse = 0.4;
	scene2.light_color = vec3(1,1,1);

	Icosahedron->initTriangles(20,icosahedron, vec3(1,0,0), 0.5, 1);
	scene2.shapesInScene.push_back(Icosahedron);

	sphere2->material.specularStrength = 64;
	scene2.shapesInScene.push_back(sphere2);

	sphere3->material.specularStrength = 64;
	scene2.shapesInScene.push_back(sphere3);

	sphere4->material.specularStrength = 64;
	scene2.shapesInScene.push_back(sphere4);

	greenCone->initTriangles(12,green_Cone, vec3(0,0.8,0), 0, 5);
	greenCone->material.specularStrength = 8;
	scene2.shapesInScene.push_back(greenCone);

	scene2.shapesInScene.push_back(floorTwo);

	backWallTwo->material.specularStrength = 8;
	scene2.shapesInScene.push_back(backWallTwo);

	return scene2;
}


//Scene 3
vec3 diamond[]{
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
	std::shared_ptr<Triangles> Diamond = std::make_shared<Triangles>();

	std::shared_ptr<Plane> Floor3 = std::make_shared<Plane>(
			vec3(0,-1,0), vec3(0,1,0),vec3(0.8,0.8,0.8), 0, 1);

	std::shared_ptr<Plane> backWall3 = std::make_shared<Plane>(
			vec3(0,0,-12), vec3(0,0,1), vec3(0.6,0.7,1), 0,2);

	std::shared_ptr<Plane> leftWall3 = std::make_shared<Plane>(
			vec3(-2,0,0), vec3(1,0,0), vec3(0.8,0.6,0.9), 0,3);

	std::shared_ptr<Sphere> sphere5 = std::make_shared<Sphere>(
			vec3( -1, -0.25, -6), 0.75, vec3(0.4,0.1,1), 0.3, 4);
	std::shared_ptr<Sphere> sphere6 = std::make_shared<Sphere>(
			vec3( 0.6, 0.4, -2.5), 0.4, vec3(0.2,0.5,0.9), 0.5, 5);
	std::shared_ptr<Sphere> sphere7 = std::make_shared<Sphere>(
			vec3( -0.5, -0.75, -4.5), 0.25, vec3(0.4,0.6,0.8), 0, 6);

	scene3.light = vec3(4,6,-1);
	scene3.ambient = 0.1;
	scene3.diffuse = 0.4;
	scene3.light_color = vec3(1,1,1);

	scene3.shapesInScene.push_back(Floor3);

	backWall3->material.specularStrength = 8;
	scene3.shapesInScene.push_back(backWall3);

	leftWall3->material.reflectionStrength = 0.2;
	scene3.shapesInScene.push_back(leftWall3);

	sphere5->material.specularStrength = 16;
	scene3.shapesInScene.push_back(sphere5);

	sphere6->material.specularStrength = 8;
	scene3.shapesInScene.push_back(sphere6);

	sphere7->material.specularStrength = 64;
	scene3.shapesInScene.push_back(sphere7);

	Diamond->initTriangles(4, diamond, vec3(0,0.85,0.95), 0.3, 7);
	Diamond->material.specularStrength = 8;
	scene3.shapesInScene.push_back(Diamond);
	return scene3;
}
