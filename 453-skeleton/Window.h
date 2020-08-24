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
	virtual void windowSizeCallback(int width, int height) { glViewport(0, 0, width, height); }
};


class Window {

public:
	Window(int width, int height, const char* title, GLFWmonitor* monitor = NULL, GLFWwindow* share = NULL);
	Window(CallbackInterface* callbacks, int width, int height, const char* title, GLFWmonitor* monitor = NULL, GLFWwindow* share = NULL);

	// Copying not allowed
	Window(const Window&) = delete;
	Window operator=(const Window&) = delete;

	// Moving is allowed
	Window(Window&& other) noexcept;
	Window& operator=(Window&& other) noexcept;

	// Destructor to cleanup created window
	~Window();


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
	GLFWwindow* window;           // owning ptr from glfw
	CallbackInterface* callbacks; // non-owning optional ptr (user provided)

	static void defaultWindowSizeCallback(GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); }

	// Meta callback functions. These bind to the actual glfw callback
	// get the actual callback method from user data, and then call that.
	static void keyMetaCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouseButtonMetaCallback(GLFWwindow* window, int button, int action, int mods);
	static void cursorPosMetaCallback(GLFWwindow* window, double xpos, double ypos);
	static void scrollMetaCallback(GLFWwindow* window, double xoffset, double yoffset);
	static void windowSizeMetaCallback(GLFWwindow* window, int width, int height);
};

