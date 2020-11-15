#version 330 core

in vec3 fragPos;
in vec3 fragColor;
in vec3 n;

uniform vec3 light;

out vec4 color;

void main() {
	vec3 lightDir = normalize(light - fragPos);
    vec3 normal = normalize(n);
    float diff = max(dot(lightDir, normal), 0.0);
	color = vec4(diff * fragColor, 1.0);
}
