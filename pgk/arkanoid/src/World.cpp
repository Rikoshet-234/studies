#include "World.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>


void World::update(float delta) {
	ball.update(delta);
	paddle.update(delta);

	wall.collide(ball);

	const auto ballPos = ball.getPosition(), ballVel = ball.getVelocity();
	const auto paddlePos = paddle.getPosition();

	// Check for ball-paddle collision.
	if (ballPos.y - Ball::RADIUS <= -1.0f + Paddle::HEIGHT &&
	    ballPos.x >= paddlePos - Paddle::WIDTH / 2.0f &&
	    ballPos.x <= paddlePos + Paddle::WIDTH / 2.0f)
	{
		float angle = 30.0f * std::rand() / RAND_MAX - 15.0f;
		vec2 vel = glm::rotate(vec2(ballVel.x, ballVel.y * -1), angle);
		ball.setVelocity(vel);
	}

	// Check for ball-bottom collision (game over).
	if (ballPos.y - Ball::RADIUS <= -1.0f) {
		*this = World();
	}
}
