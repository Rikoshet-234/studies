#include "LOD.hpp"

#include <vector>
#include <stdexcept>


LOD::LOD(int size)
: size(size)
{
	glGenBuffers(LEVELS, ebo.data());
	for (int i = 0; i < LEVELS; i++) {
		initLevel(i);
	}
}


LOD::~LOD()
{
	glDeleteBuffers(LEVELS, ebo.data());
}


static void fillElements(
	int x, int y, int width, int height, int level,
	int size, std::vector<GLuint> &elements
)
{
	const auto QUAD_SIZE = 1 << level;

	int yi = y, xi = x;
	for (yi = y; yi <= y + height - QUAD_SIZE; yi += QUAD_SIZE) {
		for (xi = x; xi <= x + width - QUAD_SIZE; xi += QUAD_SIZE) {
			elements.push_back(yi * size + xi);
			elements.push_back((yi + QUAD_SIZE) * size + xi);
			elements.push_back((yi + QUAD_SIZE) * size + (xi + QUAD_SIZE));

			elements.push_back(yi * size + xi);
			elements.push_back((yi + QUAD_SIZE) * size + (xi + QUAD_SIZE));
			elements.push_back(yi * size + (xi + QUAD_SIZE));
		}
	}

	const auto YLEFT = height % QUAD_SIZE;
	const auto XLEFT = width % QUAD_SIZE;

	if (YLEFT > 0 && width - XLEFT > 0) {
		fillElements(x, yi, width - XLEFT, YLEFT, level - 1, size, elements);
	}
	if (XLEFT > 0 && height - YLEFT > 0) {
		fillElements(xi, y, XLEFT, height - YLEFT, level - 1, size, elements);
	}
	if (YLEFT > 0 && XLEFT > 0) {
		fillElements(xi, yi, XLEFT, YLEFT, level - 1, size, elements);
	}
}


void LOD::initLevel(int level)
{
	std::vector<GLuint> elements;
	fillElements(0, 0, size - 1, size - 1, level, size, elements);

	eboSize[level] = elements.size();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[level]);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		sizeof(GLuint) * elements.size(), elements.data(),
		GL_STATIC_DRAW
	);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		throw std::runtime_error("buffering LOD elements array failed");
	}
}
