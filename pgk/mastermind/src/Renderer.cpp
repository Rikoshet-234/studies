#include "Renderer.hpp"

#include <iostream>
#include <memory>


static inline GLuint makeShader(GLenum type, const char *source)
{
	GLuint shader = glCreateShader(type);

	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);

	GLint status, logLength;
	std::unique_ptr<char> logMessage;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
		logMessage = std::unique_ptr<char>(new char [logLength + 1]);
		glGetShaderInfoLog(shader, logLength, nullptr, logMessage.get());
		std::cerr << logMessage.get() << "\n";
		
		return 0;
	}

	return shader;
}


static inline GLuint makeProgram(GLenum vs, GLenum fs)
{
	GLuint program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);

	GLint status, logLength;
	std::unique_ptr<char> logMessage;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
		logMessage = std::unique_ptr<char>(new char [logLength + 1]);
		glGetProgramInfoLog(program, logLength, nullptr, logMessage.get());
		std::cerr << logMessage.get() << "\n";

		return 0;
	}

	return program;
}


bool Renderer::initShaders()
{
	GLenum error;

	// Peg shaders.
	const char *pegVSSource = R"(
#version 330 core
layout(location = 0) in vec3 vertexPosition_modelspace;
uniform vec3 pegColor;
uniform mat4 MVP;

out vec3 fragmentColor;

void main() {
	gl_Position = MVP * vec4(vertexPosition_modelspace, 1);
	fragmentColor = pegColor;
}
	)";

	const char *pegFSSource = R"(
#version 330 core
in vec3 fragmentColor;
out vec3 color;

void main() {
	color = fragmentColor;
}
	)";

	GLenum pegVS = makeShader(GL_VERTEX_SHADER, pegVSSource);
	GLenum pegFS = makeShader(GL_FRAGMENT_SHADER, pegFSSource);

	if (!(pegVS && pegFS)) {
		return false;
	}

	pegProgram = makeProgram(pegVS, pegFS);
	if (!pegProgram) {
		return false;
	}

	glDeleteShader(pegVS);
	glDeleteShader(pegFS);

	error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cerr << "Deleting peg shaders error (code: " << error << ").\n";
		return false;
	}

	pegColorUniform = glGetUniformLocation(pegProgram, "pegColor");
	mvpUniform = glGetUniformLocation(pegProgram, "MVP");

	// Board shaders.
	const char *boardVSSource = R"(
#version 330 core
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColor;

out vec3 fragmentColor;

void main() {
	gl_Position = vec4(vertexPosition_modelspace, 1);
	fragmentColor = vertexColor;
}
	)";

	const char *boardFSSource = R"(
#version 330 core
in vec3 fragmentColor;
out vec3 color;

void main() {
	color = fragmentColor;
}
	)";

	GLenum boardVS = makeShader(GL_VERTEX_SHADER, boardVSSource);
	GLenum boardFS = makeShader(GL_FRAGMENT_SHADER, boardFSSource);

	if (!(boardVS && boardFS)) {
		return false;
	}

	boardProgram = makeProgram(boardVS, boardFS);
	if (!boardProgram) {
		return false;
	}

	glDeleteShader(boardVS);
	glDeleteShader(boardFS);

	error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cerr << "Deleting board shaders error (code: " << error << ").\n";
		return false;
	}

	glLineWidth(3.0f);

	return true;
}


bool Renderer::initModels()
{
	// Create peg model.
	pegModel = {
		// Head.
		-0.75f, 1.0f, 0.0f,
		+0.75f, 1.0f, 0.0f,
		-0.75f, 0.5f, 0.0f,
		+0.75f, 1.0f, 0.0f,
		+0.75f, 0.5f, 0.0f,
		-0.75f, 0.5f, 0.0f,
		// Pin.
		-0.5, 0.5f, 0.0f,
		+0.5, 0.5f, 0.0f,
		0.0f, -1.0f, 0.0f
	};

	// Create board model.
	boardModel.clear();
	boardColors.clear();
	for (size_t y = 0; y < BOARD_HEIGHT; y++) {
		for (size_t x = 0; x < BOARD_WIDTH; x++) {
			boardModel.push_back(2.0f * x / (BOARD_WIDTH  + 2) - 1.0f);
			boardModel.push_back(1.0f - 2.0f * y / BOARD_HEIGHT);
			boardModel.push_back(0.0f);

			boardModel.push_back(2.0f * (x + 1) / (BOARD_WIDTH  + 2) - 1.0f);
			boardModel.push_back(1.0f - 2.0f * y / BOARD_HEIGHT);
			boardModel.push_back(0.0f);

			boardModel.push_back(2.0f * (x + 1) / (BOARD_WIDTH  + 2) - 1.0f);
			boardModel.push_back(1.0f - 2.0f * y / BOARD_HEIGHT);
			boardModel.push_back(0.0f);

			boardModel.push_back(2.0f * (x + 1) / (BOARD_WIDTH  + 2) - 1.0f);
			boardModel.push_back(1.0f - 2.0f * (y + 1) / BOARD_HEIGHT);
			boardModel.push_back(0.0f);

			boardModel.push_back(2.0f * (x + 1) / (BOARD_WIDTH  + 2) - 1.0f);
			boardModel.push_back(1.0f - 2.0f * (y + 1) / BOARD_HEIGHT);
			boardModel.push_back(0.0f);

			boardModel.push_back(2.0f * x / (BOARD_WIDTH  + 2) - 1.0f);
			boardModel.push_back(1.0f - 2.0f * (y + 1) / BOARD_HEIGHT);
			boardModel.push_back(0.0f);

			boardModel.push_back(2.0f * x / (BOARD_WIDTH  + 2) - 1.0f);
			boardModel.push_back(1.0f - 2.0f * (y + 1) / BOARD_HEIGHT);
			boardModel.push_back(0.0f);

			boardModel.push_back(2.0f * x / (BOARD_WIDTH  + 2) - 1.0f);
			boardModel.push_back(1.0f - 2.0f * y / BOARD_HEIGHT);
			boardModel.push_back(0.0f);
		}
	}
	boardColors.resize(boardModel.size(), 0.8f);

	boardModel.push_back(0.65f);
	boardModel.push_back(1.0f);
	boardModel.push_back(0.0f);

	boardModel.push_back(0.65f);
	boardModel.push_back(-1.0f);
	boardModel.push_back(0.0f);

	boardColors.resize(boardModel.size(), 0.3f);

	size_t size;
	GLenum error;
	
	size = pegModel.size() * sizeof(GLfloat);
	glGenBuffers(1, &peg);
	glBindBuffer(GL_ARRAY_BUFFER, peg);
	glBufferData(GL_ARRAY_BUFFER, size, pegModel.data(), GL_STATIC_DRAW);

	error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cerr << "Bufferring peg model error (code: " << error << ").\n";
		return false;
	}

	size = boardModel.size() * sizeof(GLfloat);
	glGenBuffers(1, &board);
	glBindBuffer(GL_ARRAY_BUFFER, board);
	glBufferData(GL_ARRAY_BUFFER, size, boardModel.data(), GL_STATIC_DRAW);

	error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cerr << "Bufferring board model error (code: " << error << ").\n";
		return false;
	}

	size = boardColors.size() * sizeof(GLfloat);
	glGenBuffers(1, &boardCBO);
	glBindBuffer(GL_ARRAY_BUFFER, boardCBO);
	glBufferData(GL_ARRAY_BUFFER, size, boardColors.data(), GL_STATIC_DRAW);

	error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cerr << "Bufferring board colors error (code: " << error << ").\n";
		return false;
	}

	happyModel = {
		// Left eye.
		-0.5f, 0.66f, 0.0f,
		-0.33f, 0.33f, 0.0f,
		-0.66f, 0.33f, 0.0f,
		// Right eye.
		+0.5, 0.66f, 0.0f,
		+0.66, 0.33f, 0.0f,
		+0.33, 0.33f, 0.0f,
		// Smile.
		0.0f, -0.66f, 0.0f,
		-0.66f, -0.33f, 0.0f,
		+0.66f, -0.33f, 0.0f,
	};

	size = happyModel.size() * sizeof(GLfloat);
	glGenBuffers(1, &happy);
	glBindBuffer(GL_ARRAY_BUFFER, happy);
	glBufferData(GL_ARRAY_BUFFER, size, happyModel.data(), GL_STATIC_DRAW);

	error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cerr << "Bufferring happy model error (code: " << error << ").\n";
		return false;
	}

	sadModel = {
		// Left eye.
		-0.5f, 0.66f, 0.0f,
		-0.33f, 0.33f, 0.0f,
		-0.66f, 0.33f, 0.0f,
		// Right eye.
		+0.5, 0.66f, 0.0f,
		+0.66, 0.33f, 0.0f,
		+0.33, 0.33f, 0.0f,
		// Grief.
		0.0f, -0.33f, 0.0f,
		+0.66f, -0.66f, 0.0f,
		-0.66f, -0.66f, 0.0f,
	};

	size = sadModel.size() * sizeof(GLfloat);
	glGenBuffers(1, &sad);
	glBindBuffer(GL_ARRAY_BUFFER, sad);
	glBufferData(GL_ARRAY_BUFFER, size, sadModel.data(), GL_STATIC_DRAW);

	error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cerr << "Bufferring sad model error (code: " << error << ").\n";
		return false;
	}

	faceColors = {
		// Left eye.
		0.55f, 0.55f, 1.0f,
		0.22f, 0.22f, 1.0f,
		0.22f, 0.22f, 1.0f,
		// Right eye.
		0.55f, 0.55f, 1.0f,
		0.22f, 0.22f, 1.0f,
		0.22f, 0.22f, 1.0f,
		// Mouth.
		1.0f, 0.3f, 0.3f,
		1.0f, 0.5f, 0.5f,
		1.0f, 0.5f, 0.5f
	};

	size = faceColors.size() * sizeof(GLfloat);
	glGenBuffers(1, &faceCBO);
	glBindBuffer(GL_ARRAY_BUFFER, faceCBO);
	glBufferData(GL_ARRAY_BUFFER, size, faceColors.data(), GL_STATIC_DRAW);

	error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cerr << "Bufferring face colors error (code: " << error << ").\n";
		return false;
	}
	return true;
}


vec3 Renderer::toColor(CodePeg peg)
{
	switch (peg) {
	case CodePeg::red: return vec3(1.0f, 0.0f, 0.0f);
	case CodePeg::azure: return vec3(0.0f, 1.0f, 1.0f);
	case CodePeg::green: return vec3(0.0f, 1.0f, 0.0f);
	case CodePeg::violet: return vec3(1.0f, 0.0f, 1.0f);
	case CodePeg::blue: return vec3(0.0f, 0.0f, 1.0f);
	case CodePeg::yellow: return vec3(1.0f, 1.0f, 0.0f);
	case CodePeg::none: return vec3(0.5f);
	}
	return vec3(0.5f); // To avoid gcc stupid error (clang smarter again).
}


vec3 Renderer::toColor(KeyPeg peg)
{
	switch (peg) {
	case KeyPeg::black: return vec3(0.0f, 0.0f, 0.0f);
	case KeyPeg::white: return vec3(1.0f, 1.0f, 1.0f);
	case KeyPeg::none: return vec3(0.5f);
	}
	return vec3(0.5f); // Like above, to overcome gcc stupidity.
}


Renderer::Renderer()
{
	good = initShaders() && initModels();
}


Renderer::~Renderer()
{
	glDeleteBuffers(1, &peg);
	glDeleteBuffers(1, &board);
	glDeleteBuffers(1, &boardCBO);
	glDeleteBuffers(1, &happy);
	glDeleteBuffers(1, &sad);
	glDeleteProgram(pegProgram);
}


void Renderer::drawBoard()
{
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, board);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, boardCBO);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glDrawArrays(GL_LINES, 0, boardModel.size() / 3);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}


void Renderer::drawPeg(vec3 position, vec3 color)
{
	glUniform3fv(pegColorUniform, 1, glm::value_ptr(color));

	// For now our MVP matrix is just a model matrix.
	mat4 model = glm::translate(position) * glm::scale(vec3(0.05f));
	glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, glm::value_ptr(model));
	
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, peg);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glDrawArrays(GL_TRIANGLES, 0, pegModel.size() / 3);

	glDisableVertexAttribArray(0);
}


static inline vec3 codePosition(int x, int y)
{
	return vec3(
		2.0f * (x + 0.5f) / (BOARD_WIDTH + 2) - 1.0f,
		1.0f - 2.0f * (y + 0.5f) / BOARD_HEIGHT,
		0.0f
	);
}


static inline vec3 keyPosition(int x, int y)
{
	return vec3(
		1.0f - (2.0f * (x + 0.5f) / (BOARD_WIDTH + 2)) / 4.0f,
		1.0f - 2.0f * (y + 0.5f) / BOARD_HEIGHT,
		0.0f
	);
}


void Renderer::draw(const Board &board)
{
	if (board.getState() != Board::State::playing) {
		// Use board program because we are lazy.
		glUseProgram(boardProgram);

		glEnableVertexAttribArray(0);
		const bool areWeHappy = board.getState() == Board::State::victory;
		glBindBuffer(GL_ARRAY_BUFFER, areWeHappy ? happy : sad);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, faceCBO);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

		glDrawArrays(GL_TRIANGLES, 0, faceColors.size() / 3);
		
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		return;
	}

	// Draw board.
	glUseProgram(boardProgram);
	drawBoard();

	// Draw pegs.
	glUseProgram(pegProgram);

	size_t x, y;

	// Draw previous guesses.
	x = y = 0;
	for (auto &code : board.getCodes()) {
		for (auto &peg : code) {
			if (peg != CodePeg::none) {
				drawPeg(codePosition(x, y), toColor(peg));
			}
			x++;
		}
		x = 0;
		y++;
	}
	
	// Draw current guess.
	for (auto &peg : board.getGuess()) {
		if (x == board.getHighlight()) {
			drawPeg(codePosition(x, y), toColor(peg) + vec3(0.5f, 0.5f, 0.5f));
		} else if (peg != CodePeg::none) {
			drawPeg(codePosition(x, y), toColor(peg));
		}
		x++;
	}

	// // Draw key pegs.
	x = y = 0;
	for (auto &key : board.getKeys()) {
		for (auto &peg : key) {
			if (peg != KeyPeg::none) {
				drawPeg(keyPosition(x, y), toColor(peg));
			}
			x++;
		}
		x = 0;
		y++;
	}
}
