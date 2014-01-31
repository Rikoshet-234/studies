#ifndef FILE_GL_PROGRAM_HPP
#define FILE_GL_PROGRAM_HPP


#include <GL/glew.h>
#include <GL/gl.h>

#include <istream>


namespace gl {


class Program {
private:
	GLuint program;

public:
	Program(std::initializer_list<GLuint> shaders);
	~Program();

	Program(const Program &other) = delete;
	Program & operator=(const Program &other) = delete;

	Program(Program &&other);
	Program & operator=(Program &&other);

	void swap(Program &other);

	operator GLuint() const {
		return program;
	}
};


}


#endif
