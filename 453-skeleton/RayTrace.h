//------------------------------------------------------------------------------
// NOTES:
// Normally you wouldn't get any of this file for this assignment. But this year
// is an extraordinary year and given the timeline left for the class, we are
// aiming to provide you with more than we have in previous years.
//
// The intent is to provide you with enough to implement on your own that you
// are able to learn a wide breadth of stuff about ray-tracing, but without
// requiring you to put in the time that it would normally take to write a
// ray-tracer from scratch.
//------------------------------------------------------------------------------
#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <iostream>

using namespace std;
using namespace glm;
float dot_normalized(vec3 v1, vec3 v2);
void debug(char* str, vec3 a);

struct Ray{
	vec3 p;
	vec3 d;
	Ray(vec3 point, vec3 dir){
		p = point;
		d = dir;
	}
	Ray(): p(vec3(0,0,0)), d(vec3(0,0,0))
	{}
};

struct Intersection{
	int num;
	vec3 near;
	vec3 far;
	vec3 normal;
	vec3 color;
	int id;
	float reflection;
	float spec;
	Intersection(int no, vec3 n, vec3 f, vec3 nor, vec3 cl, int ID){
		num = no;
		near = n;
		far = f;
		normal = nor;
		color = cl;
		id = ID;
	}
	Intersection(): num(0), near(0,0,0), far(0,0,0), normal(0,0,0), color(0,0,0), id(-1), reflection(0), spec(0)
	{}
};

struct Triangle{
	vec3 p1, p2, p3;
	Triangle(vec3 a, vec3 b, vec3 c){
		p1 = a;
		p2 = b;
		p3 = c;
	}
	Triangle(vec3 t[3]){
		p1 = t[0];
		p2 = t[1];
		p3 = t[2];
	}
};

class Shape{
public:
	virtual Intersection getIntersection(Ray ray) = 0;
	vec3 color;
	float reflection;	//reflection ratio
	int id;
	float spec;
	Shape(): spec(0)
	{}
};

class Triangles: public Shape{
public:
	vector<Triangle> triangles;
	Intersection getIntersection(Ray ray);
	Intersection intersectTriangle(Ray ray, Triangle t);
	void initTriangles(int num, vec3* t, vec3 cl, float rf, int ID);
};

class Sphere: public Shape{
public:
	vec3 centre;
	float radius;
	Sphere(vec3 c, float r, vec3 cl, float rf, int ID);
	Intersection getIntersection(Ray ray);
};

class Plane: public Shape{
public:
	vec3 point;
	vec3 normal;
	Plane(vec3 p, vec3 n, vec3 cl, float rf, int ID);
	Intersection getIntersection(Ray ray);
};

