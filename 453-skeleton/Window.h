#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>


class CallbackInterface {
public:
	virtual void keyCallback(int key, int scancode, int action, int mods) {}
	virtual void mouseButtonCallback(int button, int action, int mods) {}
	virtual void cursorPosCallback(double xpos, double ypos) {}
	virtual void scrollCallback(double xoffset, double yoffset) {}
};


class Window {

public:
	Window(int width, int height, const char* title, GLFWmonitor* monitor = NULL, GLFWwindow* share = NULL);
	Window(CallbackInterface* callbacks, int width, int height, const char* title, GLFWmonitor* monitor = NULL, GLFWwindow* share = NULL);

	// Copying not allowed
	Window(const Window&) = delete;
	Window operator=(const Window&) = delete;

	// Moving is allowed
	Window(Window&& other);
	Window& operator=(Window&& other);

	// Destructor to cleanup resources on GPU
	~Window();
	void dealloc();


	// Public interface
	void setCallbacks(CallbackInterface* callbacks);

	glm::ivec2 getPos() const;
	glm::ivec2 getSize() const;

	int getX() const { return getPos().x; }
	int getY() const { return getPos().y; }
	
	int getWidth() const { return getSize().x; }
	int getHeight() const { return getSize().y; }

	int shouldClose() { return glfwWindowShouldClose(window); }
	void makeContextCurrent() { glfwMakeContextCurrent(window); }
	void swapBuffers() { glfwSwapBuffers(window); }

private:
	GLFWwindow* window;         // owning ptr from glfw
	CallbackInterface* callbacks; // non-owning optional ptr (user provided)
};

