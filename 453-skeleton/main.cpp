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
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


using ControlPoints = std::list<glm::vec3>;
using ParametricCurve = std::function<glm::vec3(float, std::vector<glm::vec3> const&)>;

CPU_Geometry controlPointGeometry(ControlPoints const &cp, glm::vec3 color) {
	CPU_Geometry retVal;
	retVal.verts.resize(cp.size());
	retVal.colors.resize(cp.size(), color);
	std::copy(cp.begin(), cp.end(), retVal.verts.begin());
	return retVal;
}

ControlPoints::iterator findClosest(ControlPoints &cp, glm::vec3 searchPoint) {
	ControlPoints::iterator retVal = cp.begin();
	float dist = std::numeric_limits<float>::max();
	for (ControlPoints::iterator cur = cp.begin(); cur != cp.end(); ++cur) {
		float newDist = glm::length(searchPoint - (*cur));
		if (newDist < dist) {
			retVal = cur;
			dist = newDist;
		}
	}
	return retVal;
}

glm::vec3 lerp(float t, glm::vec3 a, glm::vec3 b) {
	return t*b + (1-t)*a;
}

glm::vec3 delCastelJau(float t, std::vector<glm::vec3> const &p) {
	if (p.size() == 1) return p[0];
	std::vector<glm::vec3> newPoints;
	for(size_t i = 1; i < p.size(); ++i) {
		newPoints.push_back(lerp(t, p[i-1], p[i]));
	}
	return delCastelJau(t, newPoints);
}

std::vector<glm::vec3> sampleCurve(size_t samples, std::vector<glm::vec3> const &initialPoints, ParametricCurve f) {
	std::vector<glm::vec3> retval;
	for (size_t i = 0; i < samples; ++i) {
		float u = float(i) / float(samples-1);
		retval.push_back(f(u, initialPoints));
	}
	return retval;
}

glm::vec3 windowToGL(float xpos, float ypos, float windowWidth, float windowHeight) {
	float halfWindowW = windowWidth/2.f;
	float halfWindowH = windowHeight/2.f;
	return glm::vec3{
		(xpos-halfWindowW)/halfWindowW,
		-((ypos-halfWindowH)/halfWindowH),
		0.0f
	};
}

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

void updateGPUGeometry(GPU_Geometry &gpuGeom, CPU_Geometry const &cpuGeom) {
	gpuGeom.bind();
	gpuGeom.setVerts(cpuGeom.verts);
	gpuGeom.setColors(cpuGeom.colors);
}

enum class ButtonState {
	CLICKED,
	DRAGGING,
	NOT_CLICKED
};
class CurveEditor : public Scene {
	public:
		CurveEditor(ParametricCurve const &c)
			: controlPoints()
			, currentPoint(controlPoints.end())
			, leftButton(ButtonState::NOT_CLICKED)
			, rightButton(ButtonState::NOT_CLICKED)
			, cpPointsGeom()
			, cpLinesGeom()
			, curveGeom()
			, shader("shaders/test.vert", "shaders/test.frag")
			, parametricEvaluator(c)
			, P(1.0f)
			, V(1.0f)
		{
		}

		void updateGPUGeom() {
			updateGPUGeometry(cpPointsGeom, controlPointGeometry(controlPoints, glm::vec3{1.0, 0.0, 0.0}));
			updateGPUGeometry(cpLinesGeom, controlPointGeometry(controlPoints, glm::vec3{0.0, 1.0, 0.0}));
			if (controlPoints.size() > 2) {
				updateGPUGeometry(curveGeom, generateCurveGeometry(targetSamples, glm::vec3{0.f, 0.f, 0.f}));
			}
		}

		CPU_Geometry generateCurveGeometry(size_t samples, glm::vec3 color) {
			CPU_Geometry retval;
			retval.colors.resize(samples, color);

			retval.verts = sampleCurve(samples, getControlPointsVec(), parametricEvaluator);

			return retval;
		}

		virtual void keyCallback(int key, int scancode, int action, int mods) {
		}

		virtual void mouseButtonCallback(int button, int action, int mods) {
			auto p = windowToGL(curXPos, curYPos, windowWidth, windowHeight);
			if(action == GLFW_PRESS) {
				if (button == GLFW_MOUSE_BUTTON_LEFT) {
					leftButton = ButtonState::CLICKED;
					auto closest = findClosest(controlPoints, p);
					if (closest != controlPoints.end() && glm::length(*closest - p) < clickRange) {
						currentPoint = closest;
					} else {
						controlPoints.push_back(p);
						updateGPUGeom();
					}
				}
				if (button == GLFW_MOUSE_BUTTON_RIGHT) {
					rightButton = ButtonState::CLICKED;
					auto closest = findClosest(controlPoints, p);
					if (closest != controlPoints.end() && glm::length(*closest - p) < clickRange) {
						controlPoints.erase(closest);
						updateGPUGeom();
					}
				}
			} else if(action == GLFW_RELEASE) {
				if (button == GLFW_MOUSE_BUTTON_LEFT) {
					leftButton = ButtonState::NOT_CLICKED;
				}
				if (button == GLFW_MOUSE_BUTTON_RIGHT) {
					rightButton = ButtonState::NOT_CLICKED;
				}
			}
		}
		virtual void cursorPosCallback(double xpos, double ypos) {
			curXPos = xpos;
			curYPos = ypos;
		}

		virtual void render() {
			glPointSize(10.0f);
			glLineWidth(5.0f);
			shader.use();
			glm::mat4 M(1.0f);
			auto location = glGetUniformLocation(shader, "M");
			glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(M));
			location = glGetUniformLocation(shader, "V");
			glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(V));
			location = glGetUniformLocation(shader, "P");
			glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(P));

			cpPointsGeom.bind();
			glDrawArrays(GL_POINTS, 0, GLsizei(controlPoints.size()));

			cpLinesGeom.bind();
			glDrawArrays(GL_LINE_STRIP, 0, GLsizei(controlPoints.size()));

			if (controlPoints.size() > 2) {
				curveGeom.bind();
				glDrawArrays(GL_LINE_STRIP, 0, GLsizei(targetSamples));
			}

		}

		std::vector<glm::vec3> getControlPointsVec() {
			std::vector<glm::vec3> points(controlPoints.size());
			std::copy(controlPoints.begin(), controlPoints.end(), points.begin());
			return points;
		}

		ControlPoints getControlPoints() {
			return controlPoints;
		}

	private:
		ControlPoints controlPoints;
		ControlPoints::iterator currentPoint;
		ButtonState leftButton;
		ButtonState rightButton;

		GPU_Geometry cpPointsGeom;
		GPU_Geometry cpLinesGeom;
		GPU_Geometry curveGeom;
		ShaderProgram shader;

		ParametricCurve parametricEvaluator;
		glm::mat4 P;
		glm::mat4 V;


		float curXPos;
		float curYPos;
		float clickRange = 0.010f;

		size_t targetSamples = 1000;

};

class SurfaceOfRevolution : public Scene {
	public:
		SurfaceOfRevolution(ControlPoints const &cp, ParametricCurve c)
			: controlPoints(cp)
			, surfaceGeom()
			, shader("shaders/test.vert", "shaders/test.frag")
			, parametricEvaluator(c)
			, P(glm::perspective(glm::radians(15.0f), 1.0f, 0.1f, 100.0f))
			, V(glm::lookAt(
					glm::vec3(0.0, 5.0, -5.0),
					glm::vec3(0.0, 0.0, 0.0),
					glm::vec3(0.0, 1.0, 0.0)
				)
			)

		{
			updateGPUGeom();
		}

		virtual void keyCallback(int key, int scancode, int action, int mods) {
			if (key == GLFW_KEY_T && action == GLFW_PRESS) {
				nicer = !nicer;
			}
		}


		void updateGPUGeom() {
			if (controlPoints.size() > 2) {
				updateGPUGeometry(
					surfaceGeom,
					sampleToSurface(glm::vec3{1.0f, 1.0f, 1.0f})
				);
				updateGPUGeometry(
					surfaceWireGeometry,
					sampleToSurface(glm::vec3{0.f, 0.f, 0.f})
				);
			}
		}

		CPU_Geometry sampleToSurface(glm::vec3 color) {
			CPU_Geometry retval;

			std::vector<glm::vec3> initialPoints(controlPoints.size());
			std::copy(controlPoints.begin(), controlPoints.end(), initialPoints.begin());
			auto initialVerts = sampleCurve(targetCurveSamples, initialPoints, parametricEvaluator);
			auto previousVerts = initialVerts;

			for (size_t i = 1; i < targetCircleSamples; ++i) {
				float v = float(i) / float(targetCircleSamples-1);
				v *= (2*M_PI);
				std::vector<glm::vec3> rotatedVerts(initialVerts.size());
				for(size_t j = 0; j < initialVerts.size(); ++j) {
					rotatedVerts[j] = glm::rotateY(initialVerts[j], v);
				}

				for(size_t j = 1; j < previousVerts.size(); ++j) {
					retval.verts.push_back(previousVerts[j-1]);
					retval.verts.push_back(previousVerts[j]);
					retval.verts.push_back(rotatedVerts[j-1]);

					retval.verts.push_back(rotatedVerts[j-1]);
					retval.verts.push_back(previousVerts[j]);
					retval.verts.push_back(rotatedVerts[j]);
				}

				previousVerts = rotatedVerts;
			}
			numVertices = retval.verts.size();
			retval.colors.resize(numVertices, color);
			return retval;
		}


		virtual void render() {
			//glEnable(GL_CULL_FACE);
			//glCullFace(GL_BACK);
			glLineWidth(2.0f);
			shader.use();
			auto location = glGetUniformLocation(shader, "V");
			glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(V));
			location = glGetUniformLocation(shader, "P");
			glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(P));
			if (controlPoints.size() > 2) {

				glm::mat4 M(1.0f);
				location = glGetUniformLocation(shader, "M");
				if (nicer) {
					surfaceGeom.bind();
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(M));
					glDrawArrays(GL_TRIANGLES, 0, GLsizei(numVertices));

					glEnable (GL_DEPTH_TEST);
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					surfaceWireGeometry.bind();

					M = glm::translate(glm::vec3(0, 0, -0.001));
					glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(M));
					glDrawArrays(GL_TRIANGLES, 0, GLsizei(numVertices));
				} else {

					surfaceWireGeometry.bind();
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(M));
					glDrawArrays(GL_TRIANGLES, 0, GLsizei(numVertices));
				}

			}

		}

	private:
		ControlPoints controlPoints;
		GPU_Geometry surfaceGeom;
		GPU_Geometry surfaceWireGeometry;
		ShaderProgram shader;
		ParametricCurve parametricEvaluator;

		glm::mat4 P;
		glm::mat4 V;

		size_t targetCurveSamples = 25;
		size_t targetCircleSamples = 25;
		size_t numVertices = 0;
		bool nicer = true;
};

 //glm::rotateY 	( 	detail::tvec3< T > const &  	v, T const &  	angle)


// EXAMPLE CALLBACKS
class Assignment3 : public CallbackInterface {

public:
	Assignment3()
		: bezierScene(std::make_unique<CurveEditor>(delCastelJau))
		, currentScene(nullptr)
	{
		currentScene = bezierScene.get();
	}

	virtual void keyCallback(int key, int scancode, int action, int mods) {
		if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
			currentScene = bezierScene.get();
		}
		if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
			sorScene = std::make_unique<SurfaceOfRevolution>(bezierScene->getControlPoints(), delCastelJau);
			currentScene = sorScene.get();
		}
		if (key == GLFW_KEY_F && action == GLFW_PRESS) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		if (key == GLFW_KEY_W && action == GLFW_PRESS) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		if (currentScene) {
			currentScene->keyCallback(key, scancode, action, mods);
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
	std::unique_ptr<CurveEditor> bezierScene;
	std::unique_ptr<SurfaceOfRevolution> sorScene;

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
