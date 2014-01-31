#version 330
uniform vec3 position;
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColor;
out vec3 fragmentColor;

void main() {
	gl_Position = vec4(vertexPosition_modelspace + position, 1);
	fragmentColor = vertexColor;
}
