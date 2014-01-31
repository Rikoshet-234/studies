#include "Wall.hpp"


enum class Collision {
	vertical,
	horizontal,
	both,
	none
};


static inline Collision collision(vec2 ball, vec2 brick)
{
	vec2 delta = vec2(std::abs(ball.x - brick.x), std::abs(ball.y - brick.y));

	if (delta.x > Wall::BRICK_WIDTH / 2.0f + Ball::RADIUS ||
	    delta.y > Wall::BRICK_HEIGHT / 2.0f + Ball::RADIUS)
	{
		return Collision::none;		
	}

	if (delta.x <= Wall::BRICK_WIDTH / 2.0f) {
		return Collision::horizontal;
	}
	if (delta.y <= Wall::BRICK_HEIGHT / 2.0f) {
		return Collision::vertical;
	}

	const float dx = delta.x - Wall::BRICK_WIDTH / 2.0f;
	const float dy = delta.y - Wall::BRICK_HEIGHT / 2.0f;
	const float r = Ball::RADIUS;
	if (dx * dx + dy * dy <= r * r) {
		return Collision::both;
	}

	return Collision::none;
}


void Wall::collide(Ball &ball)
{
	// If ball is below wall, there certainly is no collision.
	if (ball.getPosition().y + Ball::RADIUS < 1.0f - Wall::HEIGHT) {
		return;
	}

	// For now we are checking for all bricks (which is obviously dumb).
	for (auto y = 0; y < Wall::ROWS; y++) {
		for (auto x = 0; x < Wall::COLUMNS; x++) {
			auto index = y * Wall::COLUMNS + x;
			if (bricks[index] <= 0) {
				continue;
			}

			auto cx = (x + 0.5f) * Wall::BRICK_WIDTH - 1.0f;
			auto cy = 1.0f - (y + 0.5f) * Wall::BRICK_HEIGHT;
			switch (collision(ball.getPosition(), vec2(cx, cy))) {
			case Collision::horizontal:
				ball.velocity.y *= -1;
				bricks[index] = 0;
				break;
			case Collision::vertical:
				ball.velocity.x *= -1;
				bricks[index] = 0;
				break;
			case Collision::both:
				ball.velocity *= -1;
				bricks[index] = 0;
			case Collision::none:
				break;
			}
		}
	}
}
