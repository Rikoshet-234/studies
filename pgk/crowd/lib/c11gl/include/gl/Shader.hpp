#ifndef GL_SHADER_H
#define GL_SHADER_H


#include <iostream>
#include <fstream>
#include <string>
#include <memory>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>


namespace gl {


enum shader_type {
	VertexShader,
	FragmentShader
};


class Shader {
private:
	GLuint shader;
	bool good = false;

	void init(shader_type type, std::istream &is);

public:
	Shader();
	Shader(shader_type type, std::istream &is);
	Shader(shader_type type, const char *filename);
	~Shader();

	Shader(Shader &&other);
	Shader(const Shader &other) = delete;

	Shader & operator=(Shader &&other);
	Shader & operator=(const Shader &other) = delete;

	operator GLuint() const {
		return shader;
	}

	operator bool() const {
		return good;
	}
};


}


#endif
