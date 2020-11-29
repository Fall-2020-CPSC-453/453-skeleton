#define _USE_MATH_DEFINES
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <execution>
#include <string>

#include "Geometry.h"
#include "GLDebug.h"
#include "Log.h"
#include "ShaderProgram.h"
#include "Shader.h"
#include "Texture.h"
#include "Window.h"
#include "imagebuffer.h"
#include "RayTrace.h"
#include "Scene.h"
#include "Lighting.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"


int hasIntersection(Scene const &scene, Ray ray, int skipID){
	for (auto &shape : scene.shapesInScene) {
		Intersection tmp = shape->getIntersection(ray);
		if(
			shape->id != skipID
			&& tmp.num!=0
			&& glm::length(tmp.near - ray.origin) > 0.00001
			&& glm::length(tmp.near - ray.origin) < glm::length(ray.origin - scene.light) - 0.01
		){
			return tmp.id;
		}
	}
	return -1;
}

Intersection getClosestIntersection(Scene const &scene, Ray ray, int skipID){ //get the nearest
	Intersection closestIntersection;
	float min = std::numeric_limits<float>::max();
	for(auto &shape : scene.shapesInScene) {
		if(skipID == shape->id) {
			// Sometimes you need to skip certain shapes.
			continue;
		}
		Intersection p = shape->getIntersection(ray);
		float distance = glm::length(p.near - ray.origin);
		if(p.num !=0 && distance < min){
			min = distance;
			closestIntersection = p;
		}
	}
	return closestIntersection;
}

Ray reflectRay(Ray const &r, glm::vec3 p, glm::vec3 normal) {
	// From: http://paulbourke.net/geometry/reflected/
	// Rr = Ri - 2 N (Ri . N)
	auto N = glm::normalize(normal);
	auto Ri = glm::normalize(r.direction);
	float s = glm::dot(Ri, N);
	auto Rr = Ri - 2.f * N * s;

	return Ray(p, Rr);
}

glm::vec3 raytraceSingleRay(Scene const &scene, Ray const &ray, int level, int source_id) {
	Intersection result = getClosestIntersection(scene, ray, source_id); //find intersection

	FragmentShadingParameters params;
	params.point = result.near;
	params.pointNormal = result.normal;
	params.rayOrigin = ray.origin;
	params.lightPosition = scene.light;
	params.sceneAmbient = scene.ambient;
	params.sceneDiffuse = scene.diffuse;

	params.material = result.material;

	params.inShadow = false;

	if(result.num == 0) return glm::vec3(0, 0, 0); // black;

	if (level < 1) {
		params.material.reflectionStrength = 0;
	}

	if(-1!=hasIntersection(scene, Ray(result.near, scene.light-result.near), result.id)){//in shadow
		params.inShadow = true;
	}

	if (params.material.reflectionStrength > 0) {
		Ray reflection = reflectRay(ray, result.near, result.normal);
		params.reflectedColor = raytraceSingleRay(scene, reflection, level-1, result.id);
	}

	return phongShading(params);
}

struct RayAndPixel {
	Ray ray;
	int x;
	int y;
};

std::vector<RayAndPixel> getRaysForViewpoint(Scene const &scene, ImageBuffer &image, glm::vec3 viewPoint) {
	// This is the function you must implement for part 1
	//
	// This function is responsible for creating the rays that go
	// from the viewpoint out into the scene with the appropriate direction
	// and angles to produce a perspective image.
	int x = 0;
	int y = 0;
	float fov = M_PI / 7.f;
	float z = -1.f / tan(fov);
	std::vector<RayAndPixel> rays;

	for (float i = -1; x < image.Width(); x++) {
		y = 0;
		for (float j = -1; y < image.Height(); y++) {
			Ray r = Ray(viewPoint, glm::normalize(vec3(i-viewPoint.x, j-viewPoint.y, z-viewPoint.z)));
			rays.push_back({r, x, y});
			j += 2.f / image.Height();
		}
		i += 2.f / image.Width();
	}
	return rays;
}

void raytraceImage(Scene const &scene, ImageBuffer &image, glm::vec3 viewPoint) {
	// Reset the image to the current size of the screen.
	image.Initialize();

	// Get the set of rays to cast for this given image / viewpoint
	std::vector<RayAndPixel> rays = getRaysForViewpoint(scene, image, viewPoint);


	// This loops processes each ray and stores the resulting pixel in the image.
	// final color into the image at the appropriate location.
	//
	// I've written it this way, because if you're keen on this, you can
	// try and parallelize this loop to ensure that your ray tracer runs as
	// fast as possible
	//
	// Note, if you do this, you will need to be careful about how you render
	// things below too
	std::for_each(std::execution::par, std::begin(rays), std::end(rays), [&] (auto const &r) {
		glm::vec3 color = raytraceSingleRay(scene, r.ray, 5, -1);
		image.SetPixel(r.x, r.y, color);
	});
}

// EXAMPLE CALLBACKS
class Assignment5 : public CallbackInterface {

public:
	Assignment5() {
		viewPoint = glm::vec3(0, 0, 0);
		scene = initScene2();
		raytraceImage(scene, outputImage, viewPoint);
	}

	virtual void keyCallback(int key, int scancode, int action, int mods) {
		if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
			shouldQuit = true;
		}

		if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
			scene = initScene1();
			raytraceImage(scene, outputImage, viewPoint);
		}

		if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
			scene = initScene2();
			raytraceImage(scene, outputImage, viewPoint);
		}

		if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
			scene = initScene3();
			raytraceImage(scene, outputImage, viewPoint);
		}
	}

	bool shouldQuit = false;

	ImageBuffer outputImage;
	Scene scene;
	glm::vec3 viewPoint;

};
// END EXAMPLES


int main() {
	Log::debug("Starting main");

	// WINDOW
	glfwInit();

	// Change your image/screensize here.
	int width = 800;
	int height = 800;
	Window window(width, height, "CPSC 453");

	GLDebug::enable();

	// CALLBACKS
	std::shared_ptr<Assignment5> a5 = std::make_shared<Assignment5>(); // can also update callbacks to new ones
	window.setCallbacks(a5); // can also update callbacks to new ones

	// RENDER LOOP
	while (!window.shouldClose() && !a5->shouldQuit) {
		glfwPollEvents();

		glEnable(GL_FRAMEBUFFER_SRGB);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		a5->outputImage.Render();

		window.swapBuffers();
	}


	// Save image to file:
	// outpuImage.SaveToFile("foo.png")

	glfwTerminate();
	return 0;
}
