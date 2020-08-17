#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Shader.h"

int main() {

	// GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // needed for mac?

	GLFWwindow* window = glfwCreateWindow(800, 600, "CPSC 4534", NULL, NULL);
	if (window == NULL) {
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// GLEW
	GLenum err = glewInit();
	if (glewInit() != GLEW_OK) {
		std::cerr << "glewInit error: " << glewGetErrorString(err) << std::endl;
		return -1;
	}

	// SHADERS
	Shader shader("shaders/test.vert", "shaders/test.frag");

	// Testing move and delete
	{
		Shader shader2("shaders/test.vert", "shaders/test.frag");
		shader = std::move(shader2);
	}

	// SHADER COPY AND MOVE
	//Shader deffcons; // not allowed
	//Shader copycons(shader); // not allowed
	//Shader copyassn = shader; // not allowed
	//Shader movecons(std::move(shader)); // allowed
	//Shader moveassn = std::move(shader); // allowed

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


	// CALLBACKS
	auto keyCallback = [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (key == GLFW_KEY_R && action == GLFW_PRESS) {
			Shader* shader = static_cast<Shader*>(glfwGetWindowUserPointer(window));
			shader->recompile();
		}
	};
	glfwSetWindowUserPointer(window, &shader);
	glfwSetKeyCallback(window, keyCallback);

	// RENDER LOOP
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		shader.use();
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}