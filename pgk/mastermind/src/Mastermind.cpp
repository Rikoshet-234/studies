#include <iostream>
#include <memory>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glfw.h>

#include "Board.hpp"
#include "Renderer.hpp"


static std::array<bool, 0xffff> pressed;

static void GLFWCALL onKeyPressed(int key, int action)
{
	if (action == GLFW_PRESS) {
		pressed[key] = true;
	}
}


class Mastermind {
private:

	bool good; // Is application state correct (no initalization errors, etc.)?

	GLuint vao;
	Board board;
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

		glfwSetWindowTitle("Mastermind");
		glfwEnable(GLFW_STICKY_KEYS);

		pressed.fill(false);
		glfwSetKeyCallback(onKeyPressed);

		return true;	
	}

	bool initOgl() {
		glClearColor(0.7f, 0.5f, 0.0f, 1.0f);
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		return true;
	}

	void update() {
		if (glfwGetKey(GLFW_KEY_F1) == GLFW_PRESS) {
			board.setCodePeg(CodePeg::red);
		}
		if (glfwGetKey(GLFW_KEY_F2) == GLFW_PRESS) {
			board.setCodePeg(CodePeg::azure);
		}
		if (glfwGetKey(GLFW_KEY_F3) == GLFW_PRESS) {
			board.setCodePeg(CodePeg::green);
		}
		if (glfwGetKey(GLFW_KEY_F4) == GLFW_PRESS) {
			board.setCodePeg(CodePeg::violet);
		}
		if (glfwGetKey(GLFW_KEY_F5) == GLFW_PRESS) {
			board.setCodePeg(CodePeg::blue);
		}
		if (glfwGetKey(GLFW_KEY_F6) == GLFW_PRESS) {
			board.setCodePeg(CodePeg::yellow);
		}

		if (pressed[GLFW_KEY_LEFT]) {
			board.moveLeft();
			pressed[GLFW_KEY_LEFT] = false;
		}

		if (pressed[GLFW_KEY_RIGHT]) {
			board.moveRight();
			pressed[GLFW_KEY_RIGHT] = false;
		}

		if (pressed[GLFW_KEY_ENTER]) {
			board.check();
			pressed[GLFW_KEY_ENTER] = false;
		}

	}

	void draw() {
		glClear(GL_COLOR_BUFFER_BIT);
		renderer->draw(board);
		glfwSwapBuffers();
	}

public:
	Mastermind() {
		good = initGlfw() && initOgl();
		if (!good) {
			return;
		}

		renderer = std::unique_ptr<Renderer>(new Renderer());
		good = renderer->isGood();
	}

	~Mastermind() {
		glDeleteVertexArrays(1, &vao);
		glfwTerminate();
	}

	void run() {
		while (glfwGetKey(GLFW_KEY_ESC) != GLFW_PRESS &&
		       glfwGetWindowParam(GLFW_OPENED))
		{
			update();
			draw();
		}
	}

	bool isGood() const {
		return good;
	}
};

int main()
{
	Mastermind mastermind;
	if (mastermind.isGood()) {
		mastermind.run();
	} else {
		return 1;
	}

	return 0;
}
