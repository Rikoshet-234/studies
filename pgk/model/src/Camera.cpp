#include "Camera.hpp"

#include <glm/gtx/rotate_vector.hpp>


Camera::Camera()
: position(vec2(0.0f, -5.0f))
{
	projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	updateView();
}


void Camera::strafe(float delta)
{
	position = glm::rotate(position, delta);
	updateView();
}


void Camera::zoom(float delta)
{
	float len = glm::length(position);
	position = position * (len - delta) / len;
	updateView();
}


void Camera::updateView()
{
	view = glm::lookAt(
		vec3(position, 0.0f),
		vec3(0.0f, 0.0f, 0.0f),
		vec3(0.0f, 0.0f, 1.0f)
	);
}
