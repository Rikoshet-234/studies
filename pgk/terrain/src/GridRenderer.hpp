#ifndef FILE_APPLICATION_GRID_RENDERER_HPP
#define FILE_APPLICATION_GRID_RENDERER_HPP


#include <GL/glew.h>
#include <GL/gl.h>

#include <gl/Shader.hpp>
#include <gl/Program.hpp>

#include <glm/glm.hpp>
using glm::mat4;

#include <vector>
#include <memory>
using std::unique_ptr;


class GridRenderer {
public:
	static constexpr float STEP = 2.0f;

	GridRenderer();
	~GridRenderer();
	
	GridRenderer(const GridRenderer &other) = delete;

	void draw(mat4 mvp = mat4(1.0f));

private:
	unique_ptr<gl::Program> program;
	GLint angleAttrib, mvpUniform;

	std::vector<GLfloat> mesh;
	GLuint vbo;

	void initShaders();
	void initMesh();
};


#endif
