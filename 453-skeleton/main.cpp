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


// A couple of helpers. This will take a set of points and generate a single colour
// CPU_Geometry from them
CPU_Geometry controlPointGeometry(std::vector<glm::vec3> const &cp, glm::vec3 color) {
	CPU_Geometry retVal;
	retVal.verts = cp;
	retVal.cols.resize(cp.size(), color);
	return retVal;
}

// We gave this code in one of the tutorials, so leaving it here too
void updateGPUGeometry(GPU_Geometry &gpuGeom, CPU_Geometry const &cpuGeom) {
	gpuGeom.bind();
	gpuGeom.setVerts(cpuGeom.verts);
	gpuGeom.setCols(cpuGeom.cols);
}

// Converts GLFW Window coordinates into OpenGL 2D coordinates with
// the z-axis at 0. Feel free to use this in your curve editor.
glm::vec3 windowToGL(float xpos, float ypos, float windowWidth, float windowHeight) {
	float halfWindowW = windowWidth/2.f;
	float halfWindowH = windowHeight/2.f;
	return glm::vec3{
		(xpos-halfWindowW)/halfWindowW,
		-((ypos-halfWindowH)/halfWindowH),
		0.0f
	};
}

// One possible way to organize your code
class Scene {
	public:
		Scene() {}
		virtual ~Scene() {};
		virtual void render() = 0;

		virtual void keyCallback(int key, int scancode, int action, int mods) {}
		virtual void mouseButtonCallback(int button, int action, int mods) {}
		virtual void cursorPosCallback(double xpos, double ypos) {}
		virtual void scrollCallback(double xoffset, double yoffset) {}
		virtual void windowSizeCallback(int width, int height) {
			windowWidth = float(width);
			windowHeight = float(height);
			glViewport(0, 0, width, height);
		}

	protected:
		float windowHeight;
		float windowWidth;
};

class CurveEditor : public Scene {
	public:
		CurveEditor()
			: controlPoints()
			, cpPointsGeom()
			, cpLinesGeom()
			, shader("shaders/test.vert", "shaders/test.frag")
		{
		}

		void updateGPUGeom() {
			updateGPUGeometry(cpPointsGeom, controlPointGeometry(controlPoints, glm::vec3{1.0, 0.0, 0.0}));
			updateGPUGeometry(cpLinesGeom, controlPointGeometry(controlPoints, glm::vec3{0.0, 1.0, 0.0}));
		}

		virtual void keyCallback(int key, int scancode, int action, int mods) {
			if (key == GLFW_KEY_LEFT_SHIFT) {
				if (action == GLFW_PRESS) {
					shiftDown = true;
				} else {
					shiftDown = false;
				}
			}
		}

		virtual void mouseButtonCallback(int button, int action, int mods) {
			auto p = windowToGL(curXPos, curYPos, windowWidth, windowHeight);
			if(action == GLFW_PRESS) {
				if (button == GLFW_MOUSE_BUTTON_LEFT) {
					if (!shiftDown) {
						controlPoints.push_back(p);
						updateGPUGeom();
					} else {
						// Delete point
					}
				}
			} else if(action == GLFW_RELEASE) {
				if (button == GLFW_MOUSE_BUTTON_LEFT) {
				}
			}
		}
		virtual void cursorPosCallback(double xpos, double ypos) {
			curXPos = xpos;
			curYPos = ypos;
		}

		virtual void render() {
			// Note these only work on some systems, unfortunately.
			// In order for them to work, you have to comment out line 60
			// If you're on a mac, you can't comment out line 60, so you
			// these will have no effect. :(
			glPointSize(10.0f);
			glLineWidth(5.0f);
			shader.use();

			cpPointsGeom.bind();
			glDrawArrays(GL_POINTS, 0, GLsizei(controlPoints.size()));

			cpLinesGeom.bind();
			glDrawArrays(GL_LINE_STRIP, 0, GLsizei(controlPoints.size()));

		}

	private:
		std::vector<glm::vec3> controlPoints;

		GPU_Geometry cpPointsGeom;
		GPU_Geometry cpLinesGeom;
		ShaderProgram shader;

		// Keep track of the current mouse position
		float curXPos = 0.f;
		float curYPos = 0.f;

		// Store whether the shift key has been pressed
		bool shiftDown = false;

};

// EXAMPLE CALLBACKS
class Assignment3 : public CallbackInterface {

public:
	Assignment3()
		: linesScene(std::make_unique<CurveEditor>())
		, currentScene(nullptr)
	{
		currentScene = linesScene.get();
	}

	virtual void keyCallback(int key, int scancode, int action, int mods) {
		// Handle some keys ourselves
		if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
			currentScene = linesScene.get();
		} else {
			// If we didn't handle them, pass them along to the
			// the current scene so it can handle them.
			if (currentScene) {
				currentScene->keyCallback(key, scancode, action, mods);
			}
		}
	}
	virtual void mouseButtonCallback(int button, int action, int mods) {
		if (currentScene) {
			currentScene->mouseButtonCallback(button, action, mods);
		}
	}
	virtual void cursorPosCallback(double xpos, double ypos) {
		if (currentScene) {
			currentScene->cursorPosCallback(xpos, ypos);
		}
	}
	virtual void scrollCallback(double xoffset, double yoffset) {
		if (currentScene) {
			currentScene->scrollCallback(xoffset, yoffset);
		}
	}
	virtual void windowSizeCallback(int width, int height) {
		if (currentScene) {
			currentScene->windowSizeCallback(width, height);
		}
	}

	virtual void render() {
		if (currentScene) {
			currentScene->render();
		}
	}

private:
	std::unique_ptr<CurveEditor> linesScene;

	Scene *currentScene;

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

	// RENDER LOOP
	while (!window.shouldClose()) {
		glfwPollEvents();

		glEnable(GL_FRAMEBUFFER_SRGB);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		a3->render();

		glDisable(GL_FRAMEBUFFER_SRGB); // disable sRGB for things like imgui

		window.swapBuffers();
	}

	glfwTerminate();
	return 0;
}
