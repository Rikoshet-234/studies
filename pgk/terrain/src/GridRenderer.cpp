#include "GridRenderer.hpp"

#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <stdexcept>


GridRenderer::GridRenderer()
{
	initShaders();
	initMesh();
}


GridRenderer::~GridRenderer()
{
	glDeleteBuffers(1, &vbo);
}


void GridRenderer::initShaders()
{
	gl::Shader vs(gl::VertexShader, "shaders/grid.vs");
	gl::Shader fs(gl::FragmentShader, "shaders/grid.fs");
	program = unique_ptr<gl::Program>(new gl::Program({ vs, fs }));

	mvpUniform = glGetUniformLocation(*program, "mvp");
	angleAttrib = glGetAttribLocation(*program, "angle");
}


void GridRenderer::initMesh()
{
	for (float y = -90.0f; y < +90.0f; y += STEP) {
		for (float x = -180.0f; x < +180.0; x += STEP) {
			mesh.push_back(x);
			mesh.push_back(y);
		}
	}

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(GLfloat) * mesh.size(), mesh.data(),
		GL_STATIC_DRAW
	);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		throw std::runtime_error("failed to buffer grid data");
	}
}


void GridRenderer::draw(mat4 mvp)
{
	glUseProgram(*program);

	glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, glm::value_ptr(mvp));

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(angleAttrib);

	glVertexAttribPointer(angleAttrib, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glDrawArrays(GL_POINTS, 0, mesh.size());

	glDisableVertexAttribArray(angleAttrib);
}
