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

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"


int hasIntersection(Scene const &scene, Ray ray, int skipID){
	for (auto &shape : scene.shapesInScene) {
		Intersection tmp = shape->getIntersection(ray);
		if(shape->id != skipID && tmp.num!=0 && glm::length(tmp.near - ray.p)> 0.00001 && glm::length(tmp.near - ray.p) < glm::length(ray.p - scene.light) - 0.01){
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
		float distance = glm::length(p.near - ray.p);
		if(p.num !=0 && distance < min){
			min = distance;
			closestIntersection = p;
		}
	}
	return closestIntersection;
}

glm::vec3 raytraceSingleRay(Scene const &scene, Ray const &ray, int level, int source_id) {
	vec3 color;
	Intersection result = getClosestIntersection(scene, ray, source_id); //find intersection
	if(result.num == 0)return scene.ambient* result.color;	//no intersection, dark color

	if(-1!=hasIntersection(scene, Ray(result.near, scene.light-result.near), result.id)){//in shadow
		color = scene.ambient* result.color;
	}
	else{
		float tmp_diffuse = std::max(0.0f, dot_normalized(result.normal, scene.light-result.near));//*diffuse;
		if(result.reflection == 0 || level < 1){
			if(result.spec!=0){
				return 0.7f*(scene.ambient + scene.diffuse * tmp_diffuse)* result.color + 0.3f*std::pow(dot_normalized(scene.light-result.near-ray.d, result.normal),result.spec) ;
			}
			else{
				return (scene.ambient + scene.diffuse * tmp_diffuse)* result.color;
			}
		}
		else{
			vec3 x;
			float s;
			s = dot_normalized(-ray.d, result.normal) * glm::length(ray.p - result.near);
			vec3 a = s*glm::normalize(result.normal);
			x = 2.f*a + result.near - ray.p;
			Ray r = Ray(result.near, x);

			if(result.spec!=0){
				color = (scene.ambient + scene.diffuse * tmp_diffuse)* result.color + std::pow(dot_normalized(scene.light-result.near-ray.d, result.normal),result.spec) ;
			}
			else{
				color = (scene.ambient + scene.diffuse * tmp_diffuse)* result.color;
			}
			//color = (ambient + diffuse * tmp_diffuse)* result.color + std::pow(dot_normalized(scene.light-result.near-ray.d, result.normal),result.spec) ;
			color = (1-result.reflection) * color + result.reflection * raytraceSingleRay(scene, r, level-1, result.id);
		}
	}

	return color;
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
			Ray r = Ray(viewPoint, vec3(i-viewPoint.x, j-viewPoint.y, z-viewPoint.z));
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
	std::vector<RayAndPixel> rays = getRaysForViewpoint(scene, image, viewPoint);


	// This function is responsible for processing each ray, and storing the
	// final color into the image at the appropriate location.
	//
	// I've written it this way, because if you're keen on this, you can
	// try and parallelize this loop to ensure that your ray tracer runs as
	// fast as possible
	//
	// Note, if you do this, you will need to be careful about how you render
	// things below too
	std::for_each(std::begin(rays), std::end(rays), [&] (auto const &r) {
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
