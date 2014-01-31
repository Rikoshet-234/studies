#include "Application.hpp"

#include <iostream>
#include <sstream>
#include <stdexcept>


static std::array<bool, 0xffff> pressed;

static void GLFWCALL onKeyPressed(int key, int action)
{
	if (action == GLFW_PRESS) {
		pressed[key] = true;
	}
}


Application::Application(const std::vector<const char *> &args)
{
	initGLFW();
	initGLEW();
	initGL();

	lod = unique_ptr<LOD>(new LOD(Sector::SIZE));
	offset = unique_ptr<Offset>(new Offset());
	world = unique_ptr<World>(new World(args));

	vec3 cam;
	cam = vec3(0.0f, -1.0f, 0.0f);
	globe = unique_ptr<GlobeRenderer>(new GlobeRenderer(*lod, *offset, cam));

	vec2 SW = world->getSouthWest(), NE = world->getNorthEast();
	cam = 0.5f * vec3(
		SW.x + NE.x,
		SW.y + NE.y,
		std::max(std::abs(SW.x - NE.x), std::abs(SW.y - NE.y))
	);
	flat = unique_ptr<FlatRenderer>(new FlatRenderer(*lod, *offset, cam));

	renderer = globe.get();
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

	glfwSetWindowTitle("Terrain");

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
	updateView(delta);
	updateFPS(delta);
}


void Application::updateView(float delta)
{
	// Renderer control.
	if (pressed['R']) {
		pressed['R'] = false;
		if (renderer == globe.get()) {
			renderer = flat.get();
		} else {
			renderer = globe.get();
		}
	}

	// Camera control.
	float SPEED = glfwGetKey(GLFW_KEY_LCTRL) == GLFW_PRESS ? 1.0f : 0.05f;

	if (glfwGetKey(GLFW_KEY_RIGHT) == GLFW_PRESS) {
		renderer->move(vec2(+delta, 0.0f));
	}
	if (glfwGetKey(GLFW_KEY_LEFT) == GLFW_PRESS) {
		renderer->move(vec2(-delta, 0.0f));
	}
	if (glfwGetKey(GLFW_KEY_UP) == GLFW_PRESS) {
		renderer->move(vec2(0.0f, +delta));
	}
	if (glfwGetKey(GLFW_KEY_DOWN) == GLFW_PRESS) {
		renderer->move(vec2(0.0f, -delta));
	}
	if (glfwGetKey(GLFW_KEY_PAGEUP) == GLFW_PRESS) {
		renderer->zoom(+delta * SPEED);
	}
	if (glfwGetKey(GLFW_KEY_PAGEDOWN) == GLFW_PRESS) {
		renderer->zoom(-delta * SPEED);
	}

	// LOD control.
	if (pressed['Z']) {
		pressed['Z'] = false;
		world->decreaseLOD();
	}
	if (pressed['X']) {
		pressed['X'] = false;
		world->increaseLOD();
	}
	if (pressed[GLFW_KEY_SPACE]) {
		pressed[GLFW_KEY_SPACE] = false;
		autoMode = !autoMode;
		std::cout << "LOD set to " << (autoMode ? "auto" : "manual") << ".\n";
	}
}


void Application::updateFPS(float delta)
{
	elapsed += delta;
	frames++;
	if (elapsed >= 1.0f) {
		if (autoMode && frames < FPS_LOWER_BOUND) {
			std::cout << "Decreasing level of detail.\n";
			world->decreaseFarthestLOD(renderer->getCameraPosition());
		}
		if (autoMode && frames > FPS_UPPER_BOUND) {
			std::cout << "Increasing level of detail.\n";
			world->increaseClosestLOD(renderer->getCameraPosition());
		}
		std::cout << frames << " frames per second "
		          << "(" << (1000.0f / frames) << "ms on average).\n";
		elapsed = 0.0f;
		frames = 0;
	}
}


void Application::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	renderer->draw(*world);

	glfwSwapBuffers();
}


int main(int argc, char **argv)
{
	Application app(std::vector<const char *>(argv + 1, argv + 1 + argc - 1));
	app.run();

	return 0;
}
