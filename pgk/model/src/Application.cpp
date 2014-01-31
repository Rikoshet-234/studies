#include "Application.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <array>


static std::array<bool, 0xffff> pressed;

static void GLFWCALL onKeyPressed(int key, int action)
{
	if (action == GLFW_PRESS) {
		pressed[key] = true;
	}
}


Application::Application()
{
	initGLFW();
	initGLEW();
	initGL();

	suzanne = unique_ptr<Model>(new Model("models/suzanne.obj", "textures/suzanne.png"));

	renderer = unique_ptr<Renderer>(new Renderer());
}


Application::~Application()
{
	glDeleteVertexArrays(1, &vao);
	glfwTerminate();
}


void Application::initGLFW()
{
	if (!glfwInit()) {
		throw std::runtime_error("failed to initalize GLFW");
	}

	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 2);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 1);

	if (!glfwOpenWindow(480, 480, 0, 0, 0, 0, 32, 0, GLFW_WINDOW)) {
		throw std::runtime_error("failed to create OpenGL 2.1 window");
	}

	glfwSetWindowTitle("Model viewer");

	glfwEnable(GLFW_STICKY_KEYS);
	pressed.fill(false);
	glfwSetKeyCallback(onKeyPressed);
}


void Application::initGLEW()
{
	GLenum error = glewInit();
	if (error != GLEW_OK) {
		std::ostringstream message;
		message << "failed to initalize GLEW" << " "
		        << "(" << glewGetErrorString(error) << ")";
		throw std::runtime_error(message.str());
	}
}


void Application::initGL()
{
	glClearColor(0.0f, 0.0f, 0.15f, 1.0f);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glEnable(GL_DEPTH_TEST);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		std::ostringstream message;
		message << "failed to initalize basic OpenGL values" << " "
		        << "(error code: " << error << ")";
		throw std::runtime_error(message.str());
	}
}


void Application::run()
{
	float oldTime = glfwGetTime();
	while (glfwGetKey(GLFW_KEY_ESC) != GLFW_PRESS &&
	       glfwGetWindowParam(GLFW_OPENED))
	{
		float newTime = glfwGetTime();
		float delta = newTime - oldTime;
		oldTime = newTime;

		update(delta);
		draw();
	}
}


void Application::update(float delta)
{
	renderer->update(delta);

	const float SPEED = 40.0f;
	if (glfwGetKey(GLFW_KEY_RIGHT) == GLFW_PRESS) {
		camera.strafe(+delta * SPEED);
	}
	if (glfwGetKey(GLFW_KEY_LEFT) == GLFW_PRESS) {
		camera.strafe(-delta * SPEED);
	}
	if (glfwGetKey(GLFW_KEY_UP) == GLFW_PRESS) {
		camera.zoom(+delta * SPEED * 0.10f);
	}
	if (glfwGetKey(GLFW_KEY_DOWN) == GLFW_PRESS) {
		camera.zoom(-delta * SPEED * 0.10f);
	}

	if (pressed['T']) {
		pressed['T'] = false;
		renderer->shiftTextures();
	}
	if (pressed['L']) {
		pressed['L'] = false;
		renderer->shiftLights();
	}
}


void Application::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	renderer->draw(*suzanne, camera);

	glfwSwapBuffers();
}


int main(int argc, char **argv)
{
	Application app;
	app.run();

	return 0;
}
