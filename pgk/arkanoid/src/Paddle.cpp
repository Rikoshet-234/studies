#include "Paddle.hpp"

#include <cmath>
#include <iostream>


const float Paddle::WIDTH = 0.3f;
const float Paddle::HEIGHT = 0.1f;


void Paddle::update(float delta)
{
	// Update paddle position and truncate to world borders.
	position += velocity * delta;
	if (std::abs(position) + WIDTH / 2.0f > 1.0f) {
		position = std::copysign(1.0f - WIDTH / 2.0f, position);
	}

	// Decrase paddle velocity.
	if (std::abs(velocity) > 0.01f) {
		velocity -= ((velocity > 0) - (velocity < 0)) * delta * 10.0f;
	} else {
		velocity = 0.0f;
	}
}
