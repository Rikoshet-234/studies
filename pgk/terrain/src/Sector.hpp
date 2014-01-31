#ifndef FILE_TERRAIN_SECTOR_HPP
#define FILE_TERRAIN_SECTOR_HPP


#include <GL/glew.h>
#include <GL/gl.h>

#include <istream>
#include <vector>


class Sector {
public:
	static constexpr int SIZE = 1201;

	Sector(int latitude, int longtitude, std::istream &is);
	~Sector();

	Sector(const Sector &other) = delete;

	void increaseLOD();
	void decreaseLOD();

	int getLOD() const {
		return activeLOD;
	}

	GLuint getVBO() const {
		return vbo;
	}

	int getLatitude() const {
		return latitude;
	}

	int getLongtitude() const {
		return longtitude;
	}

private:
	int activeLOD = 0;
	GLuint vbo;
	
	short latitude, longtitude;
	std::vector<GLfloat> heightmap;
};


#endif
