#ifndef FILE_RENDERER_HPP
#define FILE_RENDERER_HPP


#include <vector>
#include <memory>
using std::unique_ptr;

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

#include <gl/Shader.hpp>
#include <gl/Program.hpp>
#include <gl/ArrayBuffer.hpp>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using glm::mat4;
using glm::vec3;
using glm::vec4;

#include "Skeleton.hpp"
#include "Crowd.hpp"


class Renderer {
private:
	bool good;

	gl::Program boneProgram;
	GLuint boneMVP, boneColor;

	gl::ArrayBuffer<GLfloat> boneMesh = {
		// Top.
		-1.0f, +1.0f, +0.5f,
		+1.0f, +1.0f, +0.5f,
		0.0f, 0.0f, +1.0f,

		+1.0f, +1.0f, +0.5f,
		+1.0f, -1.0f, +0.5f,
		0.0f, 0.0f, +1.0f,

		+1.0f, -1.0f, +0.5f,
		-1.0f, -1.0f, +0.5f,
		0.0f, 0.0f, +1.0f,

		-1.0f, -1.0f, +0.5f,
		-1.0f, +1.0f, +0.5f,
		0.0f, 0.0f, +1.0f,

		// Bottom.
		-1.0f, +1.0f, +0.5f,
		+1.0f, +1.0f, +0.5f,
		0.0f, 0.0f, 0.0f,

		+1.0f, +1.0f, +0.5f,
		+1.0f, -1.0f, +0.5f,
		0.0f, 0.0f, 0.0f,

		+1.0f, -1.0f, +0.5f,
		-1.0f, -1.0f, +0.5f,
		0.0f, 0.0f, 0.0f,

		-1.0f, -1.0f, +0.5f,
		-1.0f, +1.0f, +0.5f,
		0.0f, 0.0f, 0.0f
	};

	gl::ArrayBuffer<GLfloat> boneBrightness = {
		// Top.
		0.75f, 0.75f, 0.75f,
		0.75f, 0.75f, 0.75f,
		0.25f, 0.25f, 0.25f,

		0.75f, 0.75f, 0.75f,
		0.75f, 0.75f, 0.75f,
		0.25f, 0.25f, 0.25f,

		0.75f, 0.75f, 0.75f,
		0.75f, 0.75f, 0.75f,
		0.25f, 0.25f, 0.25f,

		0.75f, 0.75f, 0.75f,
		0.75f, 0.75f, 0.75f,
		0.25f, 0.25f, 0.25f,

		// Bottom.
		0.75f, 0.75f, 0.75f,
		0.75f, 0.75f, 0.75f,
		0.25f, 0.25f, 0.25f,

		0.75f, 0.75f, 0.75f,
		0.75f, 0.75f, 0.75f,
		0.25f, 0.25f, 0.25f,

		0.75f, 0.75f, 0.75f,
		0.75f, 0.75f, 0.75f,
		0.25f, 0.25f, 0.25f,
		
		0.75f, 0.75f, 0.75f,
		0.75f, 0.75f, 0.75f,
		0.25f, 0.25f, 0.25f,
	};

	void draw(
		const std::vector<unique_ptr<Bone>> &bones,
		mat4 mvp, mat4 rstack
	);
	void draw(
		const Bone &bone,
		mat4 mvp, mat4 rstack
	);

public:
	Renderer();

	void draw(const Skeleton &skeleton, mat4 mvp);
	void draw(const Person &person, mat4 vp);
	void draw(const Crowd &crowd, mat4 vp);

	bool isGood() const {
		return good;
	}
};



#endif
