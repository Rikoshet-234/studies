#ifndef FILE_TERRAIN_OFFSET_HPP
#define FILE_TERRAIN_OFFSET_HPP


#include <GL/glew.h>
#include <GL/gl.h>

#include "Sector.hpp"


class Offset {
public:
	Offset();
	~Offset();

	GLuint getVBO() const {
		return vbo;
	}

private:
	GLuint vbo;
};


#endif
