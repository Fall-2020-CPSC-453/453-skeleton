#pragma once

#include <GL/glew.h>

#include <string>


class Shader {

public:
	Shader(std::string path, GLenum type);

	// Copying not allowed
	Shader(const Shader&) = delete;
	Shader operator=(const Shader&) = delete;

	// Moving is allowed
	Shader(Shader&& other);
	Shader& operator=(Shader&& other);

	// Destructor to cleanup resources on GPU
	~Shader();
	void dealloc();

	std::string getPath() const { return path; }
	GLenum getType() const { return type; }

	void attach(GLuint programID) { glAttachShader(programID, shaderID); }

private:
	GLuint shaderID;
	GLenum type;

	std::string path;

	bool compile();
};

