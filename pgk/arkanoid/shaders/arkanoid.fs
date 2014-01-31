#version 330
in vec3 fragmentColor;
out vec3 color;

void main() {
	color = fragmentColor;
	// color = vec3(1.0, 0.0, 0.0);
}
