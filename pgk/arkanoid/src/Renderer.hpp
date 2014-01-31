#ifndef FILE_RENDERER_H
#define FILE_RENDERER_H

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

#include <gl/Shader.hpp>
#include <gl/Program.hpp>
#include <gl/ArrayBuffer.hpp>

#include "Ball.hpp"
#include "Paddle.hpp"
#include "Wall.hpp"
#include "World.hpp"


class Renderer {
private:
	bool good = false;

	GLuint position; // Position uniform.

	static constexpr float HEX_WIDTH = 0.25f, HEX_HEIGHT = 0.25f;
	gl::ArrayBuffer<GLfloat> hexMesh = {
		-HEX_WIDTH / 4.0f, +HEX_HEIGHT / 2.0f, 0.0f,
		+HEX_WIDTH / 4.0f, +HEX_HEIGHT / 2.0f, 0.0f,
		+HEX_WIDTH / 2.0f, 0.0f, 0.0f,
		+HEX_WIDTH / 4.0f, -HEX_HEIGHT / 2.0f, 0.0f,
		-HEX_WIDTH / 4.0f, -HEX_HEIGHT / 2.0f, 0.0f,
		-HEX_WIDTH / 2.0f, 0.0f, 0.0f,
		-HEX_WIDTH / 4.0f, +HEX_HEIGHT / 2.0f, 0.0f
	};
	
	gl::ArrayBuffer<GLfloat> hexColors = {
		0.0f, 0.18f, 0.37f,
		0.0f, 0.18f, 0.37f,
		0.0f, 0.18f, 0.37f,
		0.0f, 0.18f, 0.37f,
		0.0f, 0.18f, 0.37f,
		0.0f, 0.18f, 0.37f,
		0.0f, 0.18f, 0.37f
	};

	gl::ArrayBuffer<GLfloat> paddleMesh = {
		0.0f, +Paddle::HEIGHT / 4.0f, 0.0f,
		-Paddle::WIDTH * 0.4f, 0.0f, 0.0f,
		-Paddle::WIDTH / 2.0f, +Paddle::HEIGHT, 0.0f,
		+Paddle::WIDTH / 2.0f, +Paddle::HEIGHT, 0.0f,
		+Paddle::WIDTH * 0.4f, 0.0f, 0.0f
	};

	gl::ArrayBuffer<GLfloat> paddleColors = {
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f	
	};

	gl::ArrayBuffer<GLfloat> ballMesh = {
		0.0f, 0.0f, 0.0f,
		0.0f, +Ball::RADIUS, 0.0f,
		+Ball::RADIUS, 0.0f, 0.0f,
		0.0f, -Ball::RADIUS, 0.0f,
		-Ball::RADIUS, 0.0f, 0.0f,
		0.0f, +Ball::RADIUS, 0.0f
	};
	
	gl::ArrayBuffer<GLfloat> ballColors = {
		0.8f, 0.8f, 0.8f,
		0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
	};

	gl::ArrayBuffer<GLfloat> brickMesh = {
		0.0f, 0.0f, 0.0f,
		-Wall::BRICK_WIDTH / 2.0f, +Wall::BRICK_HEIGHT / 2.0f, 0.0f,
		+Wall::BRICK_WIDTH / 2.0f, +Wall::BRICK_HEIGHT / 2.0f, 0.0f,
		+Wall::BRICK_WIDTH / 2.0f, -Wall::BRICK_HEIGHT / 2.0f, 0.0f,
		-Wall::BRICK_WIDTH / 2.0f, -Wall::BRICK_HEIGHT / 2.0f, 0.0f,
		-Wall::BRICK_WIDTH / 2.0f, +Wall::BRICK_HEIGHT / 2.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		-Wall::BRICK_WIDTH * 0.45f, +Wall::BRICK_HEIGHT * 0.35f, 0.0f,
		+Wall::BRICK_WIDTH * 0.45f, +Wall::BRICK_HEIGHT * 0.35f, 0.0f,
		+Wall::BRICK_WIDTH * 0.45f, -Wall::BRICK_HEIGHT * 0.35f, 0.0f,
		-Wall::BRICK_WIDTH * 0.45f, -Wall::BRICK_HEIGHT * 0.35f, 0.0f,
		-Wall::BRICK_WIDTH * 0.45f, +Wall::BRICK_HEIGHT * 0.35f, 0.0f,
	};
	
	gl::ArrayBuffer<GLfloat> brickColors = {
		1.0f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.0f,
		0.8f, 0.0f, 0.0f,
		0.6f, 0.0f, 0.0f,
		0.6f, 0.0f, 0.0f,
		0.6f, 0.0f, 0.0f,
		0.6f, 0.0f, 0.0f,
		0.6f, 0.0f, 0.0f,
	};

	gl::Program program;

	void drawBackground();
	void draw(const Paddle &paddle);
	void draw(const Wall &wall);
	void draw(const Ball &ball);

public:
	Renderer();

	void draw(const World &world);

	bool isGood() const {
		return good;
	}
};


#endif
