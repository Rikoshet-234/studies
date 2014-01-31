#ifndef FILE_WORLD_H
#define FILE_WORLD_H


#include <vector>

#include "Ball.hpp"
#include "Paddle.hpp"
#include "Wall.hpp"


class World {
private:
	Ball ball;
	Paddle paddle;
	Wall wall;

public:
	void update(float delta);

	Ball &getBall() {
		return ball;
	}

	const Ball &getBall() const {
		return ball;
	}

	Paddle &getPaddle() {
		return paddle;
	}

	const Paddle &getPaddle() const {
		return paddle;
	}

	Wall &getWall() {
		return wall;
	}

	const Wall &getWall() const {
		return wall;
	}
};


#endif
