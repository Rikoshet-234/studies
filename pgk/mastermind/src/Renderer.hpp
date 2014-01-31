#ifndef FILE_RENDERER_H
#define FILE_RENDERER_H


#include <vector>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using glm::vec3;
using glm::mat4;

#include "Board.hpp"


class Renderer {
private:
	std::vector<GLfloat> pegModel;
	std::vector<GLfloat> boardModel, boardColors;
	std::vector<GLfloat> happyModel, sadModel, faceColors;

	bool good;

	GLuint pegProgram, boardProgram;
	GLuint peg, board, happy, sad; // Model VBOs.
	GLuint boardCBO, faceCBO;
	GLuint pegColorUniform, mvpUniform;

	bool initModels();
	bool initShaders();

	vec3 toColor(CodePeg peg);
	vec3 toColor(KeyPeg peg);

	void drawBoard();
	void drawPeg(vec3 position, vec3 color);

public:
	Renderer();
	~Renderer();

	void draw(const Board &board);

	bool isGood() const {
		return good;
	}
};


#endif
