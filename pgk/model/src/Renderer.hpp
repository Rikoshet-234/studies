#ifndef FILE_MODEL_LOADING_HPP
#define FILE_MODEL_LOADING_HPP


#include <GL/glew.h>
#include <GL/gl.h>

#include <gl/Shader.hpp>
#include <gl/Program.hpp>

#include <glm/glm.hpp>
using glm::mat4;

#include <memory>
using std::unique_ptr;

#include "Model.hpp"
#include "Camera.hpp"


class Renderer {
public:
	Renderer();

	void update(float delta);
	void draw(const Model &model, const Camera &camera);

	void shiftTextures() {
		textureMode++;
		if (textureMode == 3) {
			textureMode = NONE;
		}
	}

	void shiftLights() {
		lightsOn = !lightsOn;
	}

private:
	bool lightsOn = true;
	enum { NONE, FILE, PROCEDURAL } textureMode = FILE;
	float timeElapsed = 0.0f;

	unique_ptr<gl::Program> program;
	GLint mvpUniform, viewUniform, samplerUniform;
	GLint textureModeUniform, lightsOnUniform, alertPhaseUniform;
	GLint positionAttrib, uvAttrib, normalAttrib;
};


#endif
