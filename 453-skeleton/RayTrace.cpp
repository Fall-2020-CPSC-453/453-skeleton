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

Intersection Triangles::intersectTriangle(Ray ray, Triangle t){
	vec3 v = t.p2 - t.p1;
	vec3 u = t.p3 - t.p1;
	vec3 n = cross(v,u);
	Intersection p;
	if(dot(n,ray.direction) == 0){
		return p;
	}
	mat3 inv = inverse(mat3(ray.direction, -u, -v));
	vec3 tmp = t.p1 - ray.origin;
	float a = inv[0].x * tmp.x + inv[1].x * tmp.y + inv[2].x * tmp.z;
	float b = inv[0].y * tmp.x + inv[1].y * tmp.y + inv[2].y * tmp.z;
	float c = inv[0].z * tmp.x + inv[1].z * tmp.y + inv[2].z * tmp.z;
	if(b>=0 && c>=0 &&b+c<=1 && a>0){
		p.num = 1;
		p.near = ray.origin + a*ray.direction;
		p.normal = n;
	}
	return p;
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


