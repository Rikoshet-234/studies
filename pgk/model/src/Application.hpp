#ifndef FILE_MODEL_LOADING_APPLICATION_HPP
#define FILE_MODEL_LOADING_APPLICATION_HPP


#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glfw.h>

#include <vector>
#include <memory>
using std::unique_ptr;

#include "Camera.hpp"
#include "Model.hpp"
#include "Renderer.hpp"


class Application {
public:
	Application();
	~Application();

	void run();
	void update(float delta);
	void draw();

private:
	GLuint vao;

	void initGLFW();
	void initGLEW();
	void initGL();

	Camera camera;
	unique_ptr<Model> suzanne;
	unique_ptr<Renderer> renderer;
};


#endif
