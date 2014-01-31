#include "Model.hpp"

#include <SOIL/SOIL.h>

#include <fstream>
#include <sstream>
#include <stdexcept>


Model::Model(const char *data, const char *texture)
{
	loadData(data);
	loadTexture(texture);
	initBuffers();
}


Model::~Model()
{
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &tbo);
	glDeleteBuffers(1, &nbo);
	glDeleteTextures(1, &texture);
}


void Model::loadData(const char *filename)
{
	std::ifstream is(filename);
	if (!is.is_open()) {
		std::ostringstream message;
		message << "could not load data \"" << filename << "\"";
		throw std::runtime_error(message.str());
	}

	std::vector<vec3> vertices;
	std::vector<vec2> uvs;
	std::vector<vec3> normals;
	std::vector<uint> faces;

	while (is) {
		std::string line;
		std::getline(is, line);

		std::istringstream ss(line);

		std::string command;
		ss >> command;		
	
		if (command == "v") {
			vec3 vertex;
			ss >> vertex.x >> vertex.y >> vertex.z;
			vertices.push_back(vertex);
		} else if (command == "vt") {
			vec2 uv;
			ss >> uv.x >> uv.y;
			uvs.push_back(uv);
		} else if (command == "vn") {
			vec3 normal;
			ss >> normal.x >> normal.y >> normal.z;
			normals.push_back(normal);
		} else if (command == "f") {
			char d = '/';
			for (auto i = 0; i < 3; i++) {
				uint vertex, uv, normal;
				ss >> vertex >> d >> uv >> d >> normal;
				faces.push_back(vertex);
				faces.push_back(uv);
				faces.push_back(normal);
			} 
		}
	}

	this->vertices.clear();
	this->uvs.clear();
	this->normals.clear();

	for (size_t i = 0; i < faces.size(); i += 3) {
		this->vertices.push_back(vertices[faces[i + 0] - 1]);
		this->uvs.push_back(uvs[faces[i + 1] - 1]);
		this->normals.push_back(normals[faces[i + 2] - 1]);
	}
}


void Model::loadTexture(const char *filename)
{
	int width, height;
	auto data = SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGB);
	if (!data) {
		std::ostringstream message;
		message << "failed to load texture \"" << filename << "\"";
		throw std::runtime_error(message.str());
	}

	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	SOIL_free_image_data(data);
}


void Model::initBuffers()
{
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(
		GL_ARRAY_BUFFER,
		vertices.size() * sizeof(vec3), vertices.data(),
		GL_STATIC_DRAW
	);

	glGenBuffers(1, &tbo);
	glBindBuffer(GL_ARRAY_BUFFER, tbo);
	glBufferData(
		GL_ARRAY_BUFFER,
		uvs.size() * sizeof(vec2), uvs.data(),
		GL_STATIC_DRAW
	);

	glGenBuffers(1, &nbo);
	glBindBuffer(GL_ARRAY_BUFFER, nbo);
	glBufferData(
		GL_ARRAY_BUFFER,
		normals.size() * sizeof(vec3), normals.data(),
		GL_STATIC_DRAW
	);
}
