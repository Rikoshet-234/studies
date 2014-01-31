#include "Ball.hpp"


const float Ball::RADIUS = 0.025f;


void Ball::update(float delta)
{
	// Update ball position and "bounce" if required.
	position += velocity * delta;
	if (std::abs(position.x) + Ball::RADIUS > 1.0f) {
		position.x = std::copysign(1.0f - Ball::RADIUS, position.x);
		velocity.x *= -1;
	}
	if (std::abs(position.y) + Ball::RADIUS > 1.0f) {
		position.y = std::copysign(1.0f - Ball::RADIUS, position.y);
		velocity.y *= -1;
	}
}
