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

	// SHADER TESTING
	Shader testShader("shaders/test.vert", "shaders/test.frag");

	// MAIN LOOP
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	std::cout << "test" << std::endl;

	glfwTerminate();
	return 0;
}