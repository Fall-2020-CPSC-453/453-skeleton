#version 330 core
out vec4 colour;

in vec3 test;

void main() {
	colour = vec4(test, 1.0f);
} 