#ifndef FILE_MODEL_LOADING_MODEL_HPP
#define FILE_MODEL_LOADING_MODEL_HPP


#include <GL/glew.h>
#include <GL/gl.h>

#include <glm/glm.hpp>
using glm::vec2;
using glm::vec3;

#include <vector>
#include <istream>


class Model {
public:
	Model(const char *data, const char *texture);
	~Model();

	GLuint getVBO() const {
		return vbo;
	}

	GLuint getTBO() const {
		return tbo;
	}

	GLuint getNBO() const {
		return nbo;
	}

	GLuint getTexture() const {
		return texture;
	}

	size_t getSize() const {
		return vertices.size();
	}

private:
	std::vector<vec3> vertices;
	std::vector<vec2> uvs;
	std::vector<vec3> normals;
	GLuint vbo, tbo, nbo;
	GLuint texture;

	void loadData(const char *filename);
	void loadTexture(const char *filename);
	void initBuffers();
};


#endif
