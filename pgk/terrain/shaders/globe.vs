#version 120

uniform mat4 mvp;
uniform int latitude;
uniform int longtitude;
attribute vec2 offset;
attribute float height;

varying vec3 color;

void main() {
	float phi = radians(float(latitude) + offset.x);
	float theta = radians(float(longtitude) + offset.y);
	gl_Position = mvp * vec4(
		cos(theta) * cos(phi),
		cos(theta) * sin(phi),
		sin(theta),
		1.0
	);

	if (height < 0.0) {
		color = vec3(0.0, 0.0, 1.0);
	} else if (height == 0.0) {
		color = vec3(1.0, 0.0, 1.0);
	} else if (height < 500.0) {
		color = vec3(0.0, height / 500.0, 0.0);
	} else if (height < 1000.0) {
		color = vec3(height / 500.0 - 1.0, 1.0, 0.0);
	} else if (height < 1500.0) {
		color = vec3(1.0, 2.0 - height / 500.0, 0.0);
	} else {
		color = vec3(1.0, 1.0, 1.0);
	}
}
