#version 120

uniform mat4 mvp;
uniform mat4 view;

attribute vec3 position;
attribute vec2 uv;
attribute vec3 normal;

varying vec3 fragmentPosition;
varying vec2 fragmentUV;
varying vec3 fragmentNormal;

varying vec3 eyeDirection;


void main() {
	gl_Position = mvp * vec4(position, 1.0);

	eyeDirection = -(view * vec4(position, 1.0)).xyz;
	
	fragmentPosition = position;
	fragmentUV = uv;
	fragmentNormal = (view * vec4(normal, 0.0)).xyz;
}
