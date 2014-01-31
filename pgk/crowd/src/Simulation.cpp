#include <iostream>
#include <memory>
#include <ctime>
using std::unique_ptr;

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glfw.h>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>
using glm::quat;
using glm::vec2;

#include "Crowd.hpp"
#include "Renderer.hpp"


enum class ViewMode {
	first_person, third_person,
	side, top_down,
	fixed
};


class Simulation {
private:
	bool good; // Is application state correct (no initalization errors, etc.)?

	GLuint vao;

	Crowd crowd;

	ViewMode viewMode;
	unique_ptr<Renderer> renderer;
	
	float cameraDistance = 5.0f;

	// Variables for FPS counting.
	size_t frames = 0; // How many frames for current second?
	float elapsed = 0.0f; // How many time elapsed since current second?


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

		glfwSetWindowTitle("Crowd simulation");
		glfwEnable(GLFW_STICKY_KEYS);

		return true;
	}

	bool initOgl() {
		glClearColor(0.0f, 0.0f, 0.15f, 1.0f);
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glEnable(GL_DEPTH_TEST);

		GLenum error = glGetError();
		if (error != GL_NO_ERROR) {
			std::cerr << "Init OpenGL error (code: " << error << ").\n";
		}

		return true;
	}

	void update(float delta) {
		crowd.update(delta);

		if (glfwGetKey(GLFW_KEY_F1) == GLFW_PRESS) {
			viewMode = ViewMode::first_person;
		}
		if (glfwGetKey(GLFW_KEY_F2) == GLFW_PRESS) {
			viewMode = ViewMode::top_down;
		}
		if (glfwGetKey(GLFW_KEY_F3) == GLFW_PRESS) {
			viewMode = ViewMode::third_person;
		}
		if (glfwGetKey(GLFW_KEY_F4) == GLFW_PRESS) {
			viewMode = ViewMode::side;
		}
		if (glfwGetKey(GLFW_KEY_F5) == GLFW_PRESS) {
			viewMode = ViewMode::fixed;
		}

		if (glfwGetKey(GLFW_KEY_UP) == GLFW_PRESS) {
			cameraDistance -= 10.0f * delta;
		}
		if (glfwGetKey(GLFW_KEY_DOWN) == GLFW_PRESS) {
			cameraDistance += 10.0f * delta;
		}
	}

	void draw() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mat4 view;

		vec3 bpos = crowd.getBatman().position;
		vec3 bvel = crowd.getBatman().velocity;

		switch (viewMode) {
		case ViewMode::side: {
			vec2 cpos =
				vec2(bpos) + glm::rotate(vec2(bvel), +90.0f) * cameraDistance;
			view = glm::lookAt(
				vec3(cpos.x, cpos.y, 0.0f),
				vec3(bpos.x, bpos.y, 0.0f),
				vec3(0.0f, 0.0f, 1.0f)
			);
			break;
		}
		case ViewMode::first_person:
			view = glm::lookAt(
				vec3(bpos.x, bpos.y, 1.8f),
				bpos + bvel + vec3(0.0f, 0.0f, 1.8f),
				vec3(0.0f, 0.0f, 1.0f)
			);
			break;
		case ViewMode::third_person:
			view = glm::lookAt(
				bpos - bvel * cameraDistance + vec3(0.0f, 0.0f, 3.0f),
				vec3(bpos.x, bpos.y, 0.0f),
				vec3(0.0f, 0.0f, 1.0f)
			);
			break;
		case ViewMode::top_down:
			view = glm::lookAt(
				vec3(bpos.x, bpos.y, cameraDistance * 2.0f),
				vec3(bpos.x, bpos.y, 0.0f),
				vec3(0.0f, 1.0f, 0.0f)
			);
			break;
		default:
			view = glm::lookAt(
				vec3(0.0f, 0.0f, cameraDistance),
				vec3(bpos.x, bpos.y, 0.0f),
				vec3(0.0f, 0.0f, 1.0f)
			);
		}

		mat4 projection = glm::perspective(45.0f, 1.0f, 0.1f, 100.0f);

		renderer->draw(crowd, projection * view);
		
		glfwSwapBuffers();
	}

public:
	Simulation() : crowd(10) {
		good = initGlfw() && initOgl();
		if (!good) {
			return;
		}

		renderer = unique_ptr<Renderer>(new Renderer());
		if (!(good = renderer->isGood())) {
			return;
		}
	}

	~Simulation() {
		glDeleteVertexArrays(1, &vao);
		glfwTerminate();
	}

	void run() {
		float oldTime = glfwGetTime();
		while (glfwGetKey(GLFW_KEY_ESC) != GLFW_PRESS &&
		       glfwGetWindowParam(GLFW_OPENED))
		{
			float newTime = glfwGetTime();
			float delta = newTime - oldTime;
			oldTime = newTime;
			
			update(delta);
			draw();

			// FPS counting.
			elapsed += delta;
			frames++;
			if (elapsed > 1.0f) {
				std::cerr << "FPS: " << frames << "\n";
				elapsed = 0.0f;
				frames = 0;
			}
		}
	}

	bool isGood() const {
		return good;
	}
};


int main()
{
	std::srand(std::time(0));

	Simulation simulation;
	if (simulation.isGood()) {
		simulation.run();
	} else {
		return 1;
	}

	return 0;
}