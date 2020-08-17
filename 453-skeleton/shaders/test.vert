#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 test;

void main() {
	test = vec3(1.0, 1.0, 0.0);
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}