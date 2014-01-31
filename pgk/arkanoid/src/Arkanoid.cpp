#include <iostream>
#include <cmath>
#include <ctime>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glfw.h>

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <gl/Shader.hpp>
#include <gl/Program.hpp>
#include <gl/ArrayBuffer.hpp>

#include "Ball.hpp"
#include "Paddle.hpp"
#include "Renderer.hpp"


class Arkanoid {
private:
	bool good; // Is application state correct (no initalization errors, etc.)?

	GLuint vao;

	World world;
	std::unique_ptr<Renderer> renderer;

	bool initGlfw() {
		if (!glfwInit()) {
			std::cerr << "Failed to initialize GLWF.\n";
			return false;
		}

		glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
		glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
		glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
		glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		if (!glfwOpenWindow(480, 480, 0, 0, 0, 0, 32, 0, GLFW_WINDOW)) {
			std::cerr << "Failed to create window with OpenGL 3.3 context.\n";
			return false;
		}

		glfwSetWindowTitle("Arkanoid");
		glfwEnable(GLFW_STICKY_KEYS);

		return true;	
	}

	bool initOgl() {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		GLenum error = glGetError();
		if (error != GL_NO_ERROR) {
			std::cerr << "Init OpenGL error (code: " << error << ").\n";
		}

		return true;
	}

	void update(float delta) {
		// Move the paddle.
		Paddle &paddle = world.getPaddle();
		if (glfwGetKey(GLFW_KEY_RIGHT) == GLFW_PRESS) {
			paddle.setVelocity(1.0f);
		}
		if (glfwGetKey(GLFW_KEY_LEFT) == GLFW_PRESS) {
			paddle.setVelocity(-1.0f);
		}

		if (glfwGetKey(GLFW_KEY_ENTER) == GLFW_PRESS) {
			const float angle = 360.0f * std::rand() / RAND_MAX;
			world.getBall().setVelocity(glm::rotate(vec2(0.0f, 1.0f), angle));
		}

		world.update(delta);
	}

	void draw() {
		glClear(GL_COLOR_BUFFER_BIT);
		renderer->draw(world);
		glfwSwapBuffers();
	}

public:
	Arkanoid() {
		good = initGlfw() && initOgl();
		if (!good) {
			return;
		}

		renderer = std::unique_ptr<Renderer>(new Renderer());
		good = renderer->isGood();
	}

	~Arkanoid() {
		glDeleteVertexArrays(1, &vao);
		glfwTerminate();
	}

	void run() {
		float oldTime = glfwGetTime();
		while (glfwGetKey(GLFW_KEY_ESC) != GLFW_PRESS &&
		       glfwGetWindowParam(GLFW_OPENED))
		{
			float newTime = glfwGetTime();
			update(newTime - oldTime);
			draw();

			oldTime = newTime;
		}
	}

	bool isGood() const {
		return good;
	}
};


int main()
{
	std::srand(std::time(0));

	Arkanoid arkanoid;
	if (arkanoid.isGood()) {
		arkanoid.run();
	} else {
		return 1;
	}

	return 0;
}