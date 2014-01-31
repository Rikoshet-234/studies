#ifndef FILE_GL_SHADER_HPP
#define FILE_GL_SHADER_HPP


#include <GL/glew.h>
#include <GL/gl.h>

#include <istream>


namespace gl {


enum shader_type {
	VertexShader,
	FragmentShader
};


class Shader {
private:
	GLuint shader;
	void init(shader_type type, std::istream &is);

public:
	Shader(shader_type type, std::istream &is);
	Shader(shader_type type, const char *filename);
	~Shader();

	Shader(const Shader &other) = delete;
	Shader &operator=(const Shader &other) = delete;

	Shader(Shader &&other);
	Shader &operator=(Shader &&other);

	void swap(Shader &other);

	operator GLuint() const {
		return shader;
	}
};


}


#endif
