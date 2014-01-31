#include <gl/Program.hpp>


namespace gl {


Program::Program()
{
}


Program::Program(std::initializer_list<GLuint> shaders)
{
	good = init(shaders);
}


Program::Program(Program &&other)
{
	program = other.program;
	good = other.good;
	other.good = false;
}


Program & Program::operator=(Program &&other)
{
	this->~Program();
	program = other.program;
	good = other.good;
	other.good = false;

	return (*this);
}


Program & Program::operator=(std::initializer_list<GLuint> shaders)
{
	this->~Program();
	good = init(shaders);

	return (*this);
}


Program::~Program()
{
	if (good) {
		glDeleteProgram(program);
	}
}


bool Program::init(std::initializer_list<GLuint> shaders)
{
	program = glCreateProgram();

	for (auto &shader : shaders) {
		glAttachShader(program, shader);
	}

	glLinkProgram(program);

	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);

	if (status == GL_TRUE) {
		return true;
	}

	GLint logLength;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

	std::unique_ptr<char> logMessage(new char[logLength + 1]);
	glGetProgramInfoLog(program, logLength, NULL, logMessage.get());
	std::cerr << logMessage.get() << "\n";
	return false;
}


}
