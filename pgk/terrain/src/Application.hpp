#ifndef FILE_TERRAIN_APPLICATION_HPP
#define FILE_TERRAIN_APPLICATION_HPP


#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glfw.h>

#include <vector>
#include <memory>
using std::unique_ptr;

#include "LOD.hpp"
#include "Offset.hpp"
#include "World.hpp"
#include "GlobeRenderer.hpp"
#include "FlatRenderer.hpp"
#include "AbstractRenderer.hpp"


class Application {
public:
	float FPS_LOWER_BOUND = 60.0f;
	float FPS_UPPER_BOUND = 200.0f;
	Application(const std::vector<const char *> &args);
	~Application();

	void run();
	void update(float delta);
	void draw();

private:
	GLuint vao;
	void initGLFW();
	void initGLEW();
	void initGL();

	unique_ptr<LOD> lod;
	unique_ptr<Offset> offset;
	unique_ptr<World> world;

	unique_ptr<GlobeRenderer> globe;
	unique_ptr<FlatRenderer> flat;
	AbstractRenderer *renderer;

	bool autoMode = false;

	float elapsed = 0.0f;
	int frames = 0;

	void updateView(float delta);
	void updateFPS(float delta);
};


#endif
