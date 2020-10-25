#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <limits>
#include <functional>

#include "Geometry.h"
#include "GLDebug.h"
#include "Log.h"
#include "ShaderProgram.h"
#include "Shader.h"
#include "Texture.h"
#include "Window.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"


// We gave this code in one of the tutorials, so leaving it here too
void updateGPUGeometry(GPU_Geometry &gpuGeom, CPU_Geometry const &cpuGeom) {
	gpuGeom.bind();
	gpuGeom.setVerts(cpuGeom.verts);
	gpuGeom.setCols(cpuGeom.cols);
}

// EXAMPLE CALLBACKS
class Assignment3 : public CallbackInterface {

public:
	Assignment3()
	{
	}

	virtual void keyCallback(int key, int scancode, int action, int mods) {
	}
	virtual void mouseButtonCallback(int button, int action, int mods) {
	}
	virtual void cursorPosCallback(double xpos, double ypos) {
	}
	virtual void scrollCallback(double xoffset, double yoffset) {
	}
	virtual void windowSizeCallback(int width, int height) {
		// The CallbackInterface::windowSizeCallback will call glViewport for us
		CallbackInterface::windowSizeCallback(width,  height);
	}

private:

};


int main() {
	Log::debug("Starting main");

	// WINDOW
	glfwInit();
	Window window(800, 800, "CPSC 453"); // can set callbacks at construction if desired


	GLDebug::enable();

	// CALLBACKS
	auto a3 = std::make_shared<Assignment3>();
	window.setCallbacks(a3);

	glEnable(GL_LINE_SMOOTH);

	ShaderProgram shader("shaders/test.vert", "shaders/test.frag");

	// The current CPU_Geometry and GPU_Geometry classes are defined in
	// Geometry.h/Geometry.cpp They will work for this assignment, but for some of
	// the bonuses you may have to modify them.
	CPU_Geometry square;
	square.verts.push_back(glm::vec3{-0.5, 0.5, 0});
	square.verts.push_back(glm::vec3{-0.5, -0.5, 0});
	square.verts.push_back(glm::vec3{0.5, -0.5, 0});
	square.verts.push_back(glm::vec3{0.5, 0.5, 0});
	square.cols.resize(square.verts.size(), glm::vec3{1.0, 0.0, 0.0});
	GPU_Geometry pointsGPUGeom;
	updateGPUGeometry(pointsGPUGeom, square);

	// Reset the colors to green
	square.cols.clear();
	square.cols.resize(square.verts.size(), glm::vec3{0.0, 1.0, 0.0});

	GPU_Geometry linesGPUGeom;
	updateGPUGeometry(linesGPUGeom, square);

	// Note these only work on some systems, unfortunately.
	// In order for them to work, you have to comment out line 60
	// If you're on a mac, you can't comment out line 60, so you
	// these will have no effect. :(
	glPointSize(10.0f);
	glLineWidth(5.0f);

	// RENDER LOOP
	while (!window.shouldClose()) {
		glfwPollEvents();

		glEnable(GL_FRAMEBUFFER_SRGB);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();

		linesGPUGeom.bind();
		glDrawArrays(GL_LINE_STRIP, 0, GLsizei(square.verts.size()));

		pointsGPUGeom.bind();
		glDrawArrays(GL_POINTS, 0, GLsizei(square.verts.size()));


		glDisable(GL_FRAMEBUFFER_SRGB); // disable sRGB for things like imgui

		window.swapBuffers();
	}

	glfwTerminate();
	return 0;
}
