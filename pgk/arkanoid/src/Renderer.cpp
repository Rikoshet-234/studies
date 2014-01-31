#include "Renderer.hpp"


Renderer::Renderer()
{
	gl::Shader vs(gl::VertexShader, "shaders/arkanoid.vs");
	gl::Shader fs(gl::FragmentShader, "shaders/arkanoid.fs");

	if (!vs || !fs) {
		std::cerr << "Renderer initalization error.\n";
		return;
	}

	program = { vs, fs };
	if (!program) {
		std::cerr << "Renderer initalization error.\n";
		return;
	}

	position = glGetUniformLocation(program, "position");

	good = true;
}


void Renderer::drawBackground()
{
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, hexMesh);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, hexColors);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	// We just simply draw tiles under screen borders are reached.
	bool even = false;
	for (auto cy = -1.0f + Renderer::HEX_HEIGHT / 2.0f;
	     cy <= 1.0f;
	     cy += Renderer::HEX_HEIGHT / 2.0f)
	{
		// Translate hex row if current row is odd.
		even = !even;
		const auto translation = even ? 0.0f : 0.75f * Renderer::HEX_WIDTH;

		for (auto cx = -1.0f + Renderer::HEX_WIDTH / 2.0f + translation;
		     cx <= 1.0f;
		     cx += (1.0f + 0.5f) * Renderer::HEX_WIDTH)
		{
			glUniform3f(position, cx, cy, 0.0f);
			glDrawArrays(GL_LINE_STRIP, 0, 7);
		}
	}

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}


void Renderer::draw(const Paddle &paddle)
{
	glUniform3f(position, paddle.getPosition(), -1.0f, 0.0f);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, paddleMesh);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, paddleColors);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 5);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}


void Renderer::draw(const Wall &wall)
{
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, brickMesh);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, brickColors);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	for (auto y = 0; y < Wall::ROWS; y++) {
		for (auto x = 0; x < Wall::COLUMNS; x++) {
			auto index = y * Wall::COLUMNS + x;
			if (wall.getBricks()[index] < 1) {
				continue;
			}

			GLfloat cx = (x + 0.5f) * Wall::BRICK_WIDTH - 1.0f;
			GLfloat cy = 1.0f - (y + 0.5f) * Wall::BRICK_HEIGHT;
			glUniform3f(position, cx, cy, 0.0f);
			glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
			glDrawArrays(GL_TRIANGLE_FAN, 6, 6);
		}
	}

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}


void Renderer::draw(const Ball &ball)
{
	glUniform3f(position, ball.getPosition().x, ball.getPosition().y, 0);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, ballMesh);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, ballColors);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 6);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}


void Renderer::draw(const World &world)
{
	glUseProgram(program);

	drawBackground();
	draw(world.getPaddle());
	draw(world.getWall());
	draw(world.getBall());
}
