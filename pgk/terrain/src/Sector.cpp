#include "Sector.hpp"
#include "LOD.hpp"

#include <stdexcept>
#include <sstream>


Sector::Sector(int latitude, int longtitude, std::istream &is)
: latitude(latitude), longtitude(longtitude)
{
	// Read binary data into a heightmap.
	const auto COUNT = Sector::SIZE * Sector::SIZE;
	heightmap.reserve(COUNT);
	for (int i = 0; i < COUNT; i++) {
		int16_t val = 0;
		is.read((char *)&val, sizeof(val));
		heightmap.push_back(val);
	}

	// Change row order.
	for (int y = 0; y < SIZE / 2; y++) {
		for (int x = 0; x < SIZE; x++) {
			std::swap(
				heightmap[y * SIZE + x],
				heightmap[(SIZE - y - 1) * SIZE + x]
			);
		}
	}

	// Send data to graphics card.
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(GLfloat) * COUNT, heightmap.data(),
		GL_STATIC_DRAW
	);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		std::ostringstream message;
		message << "failed to buffer heightmap data" << " "
		        << "(error code: " << error << ")";
		throw std::runtime_error(message.str());
	}
}


Sector::~Sector()
{
	glDeleteBuffers(1, &vbo);
}


void Sector::increaseLOD()
{
	if (activeLOD > 0) {
		activeLOD--;
	}
}

void Sector::decreaseLOD()
{
	if (activeLOD < LOD::LEVELS - 1) {
		activeLOD++;
	}
}
