#ifndef FILE_TERRAIN_LOD_HPP
#define FILE_TERRAIN_LOD_HPP


#include <GL/glew.h>
#include <GL/gl.h>

#include <array>


class LOD {
public:
	static constexpr int LEVELS = 5;

	LOD(int size);
	~LOD();

	LOD(const LOD &other) = delete;

	GLuint getEBO(int level) const {
		return ebo[level];
	}

	size_t getSize(int level) const {
		return eboSize[level];
	}

private:
	int size;
	std::array<GLuint, LEVELS> ebo;
	std::array<size_t, LEVELS> eboSize;

	void initLevel(int level);
};


#endif
