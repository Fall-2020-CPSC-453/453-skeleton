#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "ShaderProgram.h"
#include "Window.h"


// EXAMPLE CALLBACKS
class MyCallbacks : public CallbackInterface {

public:
	MyCallbacks(ShaderProgram& shader) : shader(shader) {}

	virtual void keyCallback(int key, int scancode, int action, int mods) {
		if (key == GLFW_KEY_R && action == GLFW_PRESS) {
			shader.recompile();
		}
	}

private:
	ShaderProgram& shader;
};

class MyCallbacks2 : public CallbackInterface {

public:
	MyCallbacks2() {}

	virtual void keyCallback(int key, int scancode, int action, int mods) {
		if (key == GLFW_KEY_R && action == GLFW_PRESS) {
			std::cout << "called back" << std::endl;
		}
	}
};
// END EXAMPLES


int main() {

	// WINDOW
	glfwInit();
	Window window(800, 800, "CPSC 453"); // can set callbacks at construction if desired

	// SHADERS
	ShaderProgram shader("shaders/test.vert", "shaders/test.frag");

	{
		ShaderProgram shader2("shaders/test.vert", "shaders/test.frag");
		shader = std::move(shader2);
	}

	// CALLBACKS
	MyCallbacks callbacks(shader);
	window.setCallbacks(&callbacks); // can also update callbacks to new ones

	// TEST GEOMETRY
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// RENDER LOOP
	while (!window.shouldClose()) {
		glfwPollEvents();

		shader.use();
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		window.swapBuffers();
	}

	glfwTerminate();
	return 0;
}