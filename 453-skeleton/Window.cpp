#include "Window.h"

#include <iostream>


Window::Window(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share) :
	callbacks(nullptr)
{
	// specify OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // needed for mac?

	// create window
	window = glfwCreateWindow(width, height, title, monitor, share);
	if (window == NULL) {
		std::cerr << "ERROR::WINDOW failed to create GLFW window" << std::endl;
		glfwTerminate();

		throw std::runtime_error("Failed to create GLFW window.");
	}
	glfwMakeContextCurrent(window);

	// initialize OpenGL extensions for the current context (this window)
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		std::cerr << "ERROR::WINDOW glewInit error: " << glewGetErrorString(err) << std::endl;

		throw std::runtime_error("Failed to initialize GLEW");
	}
}


Window::Window(CallbackInterface* callbacks, int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share) :
	Window(width, height, title, monitor, share)
{
	setCallbacks(callbacks);
}


Window::Window(Window&& other) :
	window(std::move(other.window)),
	callbacks(std::move(other.callbacks))
{
	other.window = nullptr;
	other.callbacks = nullptr;
}


Window& Window::operator=(Window&& other) {

	dealloc();

	window = std::move(other.window);
	callbacks = std::move(other.callbacks);

	other.window = nullptr;
	other.callbacks = nullptr;
	return *this;
}


Window::~Window() {
	dealloc();
}


void Window::dealloc() {
	glfwDestroyWindow(window);
}


void Window::setCallbacks(CallbackInterface* callbacks) {
	
	// set userdata of window to point to the object that carries out the callbacks
	glfwSetWindowUserPointer(window, callbacks);

	// Meta callback functions. These bind to the actual glfw callback
	// get the actual callback method from user data, and then call that. 
	auto keyMetaCallback = [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		CallbackInterface* callbacks = static_cast<CallbackInterface*>(glfwGetWindowUserPointer(window));
		callbacks->keyCallback(key, scancode, action, mods);
	};

	auto mouseButtonMetaCallback = [](GLFWwindow* window, int button, int action, int mods) {
		CallbackInterface* callbacks = static_cast<CallbackInterface*>(glfwGetWindowUserPointer(window));
		callbacks->mouseButtonCallback(button, action, mods);
	};

	auto cursorPosMetaCallback = [](GLFWwindow* window, double xpos, double ypos) {
		CallbackInterface* callbacks = static_cast<CallbackInterface*>(glfwGetWindowUserPointer(window));
		callbacks->cursorPosCallback(xpos, ypos);
	};

	auto scrollMetaCallback = [](GLFWwindow* window, double xoffset, double yoffset) {
		CallbackInterface* callbacks = static_cast<CallbackInterface*>(glfwGetWindowUserPointer(window));
		callbacks->scrollCallback(xoffset, yoffset);
	};

	// bind meta callbacks to actual callbacks
	glfwSetKeyCallback(window, keyMetaCallback);
	glfwSetMouseButtonCallback(window, mouseButtonMetaCallback);
	glfwSetCursorPosCallback(window, cursorPosMetaCallback);
	glfwSetScrollCallback(window, scrollMetaCallback);
}


glm::ivec2 Window::getPos() const {
	int x, y;
	glfwGetWindowPos(window, &x, &y);
	return glm::ivec2(x, y);
}


glm::ivec2 Window::getSize() const {
	int w, h;
	glfwGetWindowSize(window, &w, &h);
	return glm::ivec2(w, h);
}
