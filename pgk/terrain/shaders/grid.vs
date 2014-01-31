#version 120

uniform mat4 mvp;
attribute vec2 angle;

void main() {
	float phi = radians(angle.x);
	float theta = radians(angle.y);
	gl_Position = mvp * vec4(
		cos(theta) * cos(phi),
		cos(theta) * sin(phi),
		sin(theta),
		1.0
	);
}
