#ifndef FILE_WALL_H
#define FILE_WALL_H


#include <array>

#include "Ball.hpp"


class Wall {
public:
	static constexpr float HEIGHT = 0.5f;
	static constexpr size_t COLUMNS = 6, ROWS = 5;
	
	static constexpr float BRICK_WIDTH = 2.0 / Wall::COLUMNS;
	static constexpr float BRICK_HEIGHT = Wall::HEIGHT / Wall::ROWS;
	
	typedef std::array<int, COLUMNS * ROWS> Bricks;

private:
	Bricks bricks = {
		1, 1, 1, 1, 1, 1,
		0, 0, 0, 0, 0, 0,
		0, 2, 0, 0, 2, 0,
		0, 2, 1, 1, 2, 0,
		1, 2, 3, 3, 2, 1
	};

public:
	void collide(Ball &ball);

	const Bricks &getBricks() const {
		return bricks;
	}
};


#endif
