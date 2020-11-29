#include <iostream>
#include <glm/common.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "RayTrace.h"


using namespace std;
using namespace glm;

Sphere::Sphere(vec3 c, float r, vec3 cl, float rf, int ID){
	centre = c;
	radius = r;
	id = ID;
	material.color = cl;
	material.reflectionStrength = rf;
}

//------------------------------------------------------------------------------
// This is part 2 of your assignment. At the moment, the spheres are not showing
// up. Implement this method to make them show up.
//
// Make sure you set all of the appropriate fields in the Intersection object.
//------------------------------------------------------------------------------
Intersection Sphere::getIntersection(Ray ray){
	Intersection inter_p;
	float e = ray.origin.x - centre.x;
	float f = ray.origin.y - centre.y;
	float g = ray.origin.z - centre.z;
	float a = ray.direction.x*ray.direction.x + ray.direction.y*ray.direction.y + ray.direction.z*ray.direction.z;
	float b = 2*(ray.direction.x * e + ray.direction.y * f + ray.direction.z * g);
	float c = e*e + f*f + g*g - radius * radius;
	float delt = b*b - 4*a*c;
	inter_p.material = material;
	inter_p.id = id;
	if (delt < 0) {
		return inter_p;
	}
	else if (delt == 0) {
		if(a == 0){
			return inter_p;
		}
		else{
			float s = -b/2.f/a;
			if(s>0.001){
				inter_p.num = 1;
				inter_p.near = vec3(ray.origin.x + s*ray.direction.x, ray.origin.y + s*ray.direction.y, ray.origin.z + s*ray.direction.z);
			}
			inter_p.normal = inter_p.near-centre;
			return inter_p;
		}
	}
	else {
		if(a == 0){
			if(b == 0){
				return inter_p;
			}
			else{
				float s = -c/b;
				if(s>0.001){
					inter_p.num = 1;
					inter_p.near = vec3(ray.origin.x + s*ray.direction.x, ray.origin.y + s*ray.direction.y, ray.origin.z + s*ray.direction.z);
				}
				inter_p.normal = inter_p.near-centre;
				return inter_p;
			}
		}
		else{
			float s1, s2;
			s1 = (-b + sqrt(delt)) / 2.f / a;
			s2 = (-b - sqrt(delt)) / 2.f / a;
			if(s1>0.001 && s2>0.001){
				inter_p.num = 2;
				inter_p.far = vec3(ray.origin.x + s1*ray.direction.x, ray.origin.y + s1*ray.direction.y, ray.origin.z + s1*ray.direction.z);
				inter_p.near = vec3(ray.origin.x + s2*ray.direction.x, ray.origin.y + s2*ray.direction.y, ray.origin.z + s2*ray.direction.z);
			}
			else if(s1>0.001){
				inter_p.num = 1;
				inter_p.near = vec3(ray.origin.x + s1*ray.direction.x, ray.origin.y + s1*ray.direction.y, ray.origin.z + s1*ray.direction.z);
			}
			inter_p.normal = inter_p.near-centre;
			return inter_p;
		}
	}
}

Plane::Plane(vec3 p, vec3 n, vec3 cl, float rf, int ID){
	point = p;
	normal = n;
	id = ID;
	material.color = cl;
	material.reflectionStrength = rf;
}


float dot_normalized(vec3 v1, vec3 v2){
	return glm::dot(glm::normalize(v1), glm::normalize(v2));
}

void debug(char* str, vec3 a){
	cout << "debug:" << str << ": " << a.x <<", " << a.y <<", " << a.z << endl;
}
// --------------------------------------------------------------------------
void Triangles::initTriangles(int num, vec3 * t, vec3 cl, float rf, int ID){
	material.color = cl;
	material.reflectionStrength =rf;

	id = ID;
	for(int i = 0; i< num; i++){
		triangles.push_back(Triangle(*t, *(t+1), *(t+2)));
		t+=3;
	}
}

Intersection Triangles::intersectTriangle(Ray ray, Triangle triangle){
	// From https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
	const float EPSILON = 0.0000001;
	auto vertex0 = triangle.p1;
	auto vertex1 = triangle.p2;
	auto vertex2 = triangle.p3;

	glm::vec3 edge1, edge2, h, s, q;
	float a,f,u,v;
	edge1 = vertex1 - vertex0;
	edge2 = vertex2 - vertex0;
	h = glm::cross(ray.direction, edge2);
	a = glm::dot(edge1, h);
	if (a > -EPSILON && a < EPSILON) {
		return Intersection{}; // no intersection
	}
	f = 1.0/a;
	s = ray.origin - vertex0;
	u = f * glm::dot(s, h);
	if (u < 0.0 || u > 1.0) {
		return Intersection{}; // no intersection
	}
	q = glm::cross(s, edge1);
	v = f * glm::dot(ray.direction, q);
	if (v < 0.0 || u + v > 1.0) {
		return Intersection{}; // no intersection
	}
	// At this stage we can compute t to find out where the intersection point is on the line.
	float t = f * glm::dot(edge2, q);
	// ray intersection
	if (t > EPSILON) {
		Intersection p;
		p.near = ray.origin + ray.direction * t;
		p.normal = glm::normalize(glm::cross(edge1, edge2));
		p.material = material;
		p.num = 1;
		p.id = id;
		return p;
	} else {
		// This means that there is a line intersection but not a ray intersection.
		return Intersection{}; // no intersection
	}
}


Intersection Triangles::getIntersection(Ray ray){
	Intersection result;
	float min = 9999;
	result = intersectTriangle(ray, triangles.at(0));
	if(result.num!=0)min = distance(result.near, ray.origin);
	for(int i = 1; i<triangles.size() ;i++){
		Intersection p = intersectTriangle(ray, triangles.at(i));
		if(p.num !=0 && distance(p.near, ray.origin) < min){
			min = distance(p.near, ray.origin);
			result = p;
		}
	}

	result.material = material;

	result.id = id;
	return result;
}

Intersection Plane::getIntersection(Ray ray){
	Intersection result;
	result.material = material;
	result.id = id;
	result.normal = normal;
	if(dot(normal, ray.direction)>=0)return result;
	float s = dot(point - ray.origin, normal)/dot(ray.direction, normal);
	//if(s<0.00001)return result;
	result.num = 1;
	result.near = ray.origin + s*ray.direction;
	return result;
}


