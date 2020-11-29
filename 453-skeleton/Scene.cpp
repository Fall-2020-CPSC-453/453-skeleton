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
	sphere1->material.specular = 1.0f*sphere1->material.diffuse;
	sphere1->material.reflectionStrength = vec3(0.4);
	sphere1->material.specularCoefficient = 64;
	scene1.shapesInScene.push_back(sphere1);

	//pyramid
	std::shared_ptr<Triangles> pyramid = std::make_shared<Triangles>();
	pyramid->initTriangles(4, blue_pyramid, 2);
	pyramid->material.diffuse = vec3(0.0,0.85,0.95);
	pyramid->material.specular = 0.1f*pyramid->material.diffuse;
	pyramid->material.reflectionStrength = vec3(0.3);
	scene1.shapesInScene.push_back(pyramid);

	std::shared_ptr<Triangles> rightWall = std::make_shared<Triangles>();
	rightWall->initTriangles(2, right_wall, 3);
	rightWall->material.diffuse = vec3(0.0, 0.7, 0.0);
	rightWall->material.ambient = 0.1f*rightWall->material.diffuse;
	//rightWall->material.specular = 0.1f*rightWall->material.diffuse;
	scene1.shapesInScene.push_back(rightWall);

	std::shared_ptr<Triangles> leftWall = std::make_shared<Triangles>();
	leftWall->initTriangles(2, left_wall, 4);
	leftWall->material.diffuse = vec3(0.7, 0.0, 0.0);
	leftWall->material.ambient = 0.1f*leftWall->material.diffuse;
	//leftWall->material.specular = 0.1f*leftWall->material.diffuse;
	scene1.shapesInScene.push_back(leftWall);

	std::shared_ptr<Triangles> floorWall = std::make_shared<Triangles>();
	floorWall->initTriangles(2, floor1, 5);
	floorWall->material.diffuse = vec3(0.8, 0.8, 0.8);
	floorWall->material.ambient = 0.1f*floorWall->material.diffuse;
	//floorWall->material.specular = 0.1f*floorWall->material.diffuse;
	scene1.shapesInScene.push_back(floorWall);

	std::shared_ptr<Triangles> ceilingWall = std::make_shared<Triangles>();
	ceilingWall->initTriangles(2, ceiling, 6);
	ceilingWall->material.diffuse = vec3(1.0, 1.0, 1.0);
	ceilingWall->material.ambient = 0.1f*ceilingWall->material.diffuse;
	scene1.shapesInScene.push_back(ceilingWall);

	std::shared_ptr<Triangles> backWall = std::make_shared<Triangles>();
	backWall->initTriangles(2, back_wall, 7);
	backWall->material.diffuse = vec3(1.0, 1.0, 1.0);
	scene1.shapesInScene.push_back(backWall);

	scene1.lightPosition = vec3(0,2.5,-7.75);

	scene1.lightColor = vec3(1,1,1);
	scene1.ambientFactor = 0.1f;
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
	sphere2->material.specular = sphere2->material.diffuse;
	sphere2->material.specularCoefficient = 64;
	scene2.shapesInScene.push_back(sphere2);

	// Reflective grey sphere
	std::shared_ptr<Sphere> sphere3 = std::make_shared<Sphere>(
			vec3( 0, 1, -5), 0.4, 3);
	sphere3->material.diffuse = vec3(0.6, 0.6, 0.6);
	sphere3->material.specular = sphere3->material.diffuse;
	sphere3->material.reflectionStrength = vec3(0.5, 0.5, 0.5);
	sphere3->material.specularCoefficient = 64;
	scene2.shapesInScene.push_back(sphere3);


	// Metallic purple sphere
	std::shared_ptr<Sphere> sphere4 = std::make_shared<Sphere>(
			vec3( -0.8, -0.75, -4), 0.25, 4);
	sphere4->material.diffuse = vec3(0.4, 0.1, 1.0);
	sphere4->material.specular = sphere4->material.diffuse;
	sphere4->material.reflectionStrength = vec3(0.3);
	sphere4->material.specularCoefficient = 64;
	scene2.shapesInScene.push_back(sphere4);

	//Green cone
	std::shared_ptr<Triangles> greenCone = std::make_shared<Triangles>();
	greenCone->initTriangles(12,green_Cone, 5);
	greenCone->material.diffuse = vec3(0.0, 0.8, 0.0);
	greenCone->material.specular = greenCone->material.diffuse;
	greenCone->material.specularCoefficient = 8;
	scene2.shapesInScene.push_back(greenCone);

	//Floor
	std::shared_ptr<Plane> floorTwo = std::make_shared<Plane>(
			vec3(0,-1,0), vec3(0,1,0), 6);
	floorTwo->material.diffuse = vec3(0.8, 0.8, 0.8);
	floorTwo->material.ambient = 0.5f*floorTwo->material.diffuse;
	scene2.shapesInScene.push_back(floorTwo);

	//Back wall
	std::shared_ptr<Plane> backWallTwo = std::make_shared<Plane>(
			vec3(0,0,-12), vec3(0,0,1), 7);
	backWallTwo->material.diffuse = vec3(0.0, 0.6, 0.6);
	backWallTwo->material.ambient = 0.5f*backWallTwo->material.diffuse;
	backWallTwo->material.specular = backWallTwo->material.diffuse;
	backWallTwo->material.specularCoefficient = 8;
	scene2.shapesInScene.push_back(backWallTwo);

	scene2.lightPosition = vec3(4, 6, -1);
	scene2.lightColor = vec3(1,1,1);
	scene2.ambientFactor = 0.1f;

	return scene2;
}
