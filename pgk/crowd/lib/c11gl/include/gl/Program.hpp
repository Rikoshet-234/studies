#ifndef GL_PROGRAM_H
#define GL_PROGRAM_H


#include <iostream>
#include <memory>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>


namespace gl {


class Program {
private:
	GLuint program;
	bool good = false;

	bool init(std::initializer_list<GLuint> shaders);

public:
	Program();
	Program(std::initializer_list<GLuint> shaders);
	~Program();

	Program & operator=(std::initializer_list<GLuint> shaders);

	Program(Program &&other);
	Program(const Program &other) = delete;

	Program & operator=(Program &&other);
	Program & operator=(const Program &other) = delete;

	operator GLuint() const {
		return program;
	}

	operator bool() const {
		return good;
	}
};


}


#endif
