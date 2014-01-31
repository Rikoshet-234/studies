#include "Offset.hpp"

#include <vector>
#include <stdexcept>


Offset::Offset()
{
	const float MINUTES = 1.0 / (Sector::SIZE - 1.0);

	std::vector<GLfloat> offsets;
	for (int id = 0; id < Sector::SIZE * Sector::SIZE; id++) {
			offsets.push_back((id % Sector::SIZE) * MINUTES); // X
			offsets.push_back((id / Sector::SIZE) * MINUTES); // Y
	}

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(GLfloat) * offsets.size(), offsets.data(),
		GL_STATIC_DRAW
	);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		throw std::runtime_error("failed to buffer offset data");
	}
}


Offset::~Offset()
{
	glDeleteBuffers(1, &vbo);
}
