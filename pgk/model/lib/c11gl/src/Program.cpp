#include <gl/Program.hpp>

#include <vector>
#include <sstream>
#include <stdexcept>

namespace gl {


Program::Program(std::initializer_list<GLuint> shaders)
{
	program = glCreateProgram();

	for (auto &shader : shaders) {
		glAttachShader(program, shader);
	}

	glLinkProgram(program);

	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_TRUE) {
		return;
	}

	GLint logLength;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

	std::vector<char> logMessage(logLength + 1);
	logMessage[logLength] = 0;
	glGetProgramInfoLog(program, logLength, nullptr, logMessage.data());

	std::ostringstream message;
	message << "failed to ling program" << "\n" << logMessage.data();
	throw std::runtime_error(message.str());
}


Program::Program(Program &&other)
{
	this->swap(other);
}


Program &Program::operator=(Program &&other)
{
	this->swap(other);
	return *this;
}


void Program::swap(Program &other)
{
	using std::swap;

	swap(program, other.program);
}


Program::~Program()
{
	glDeleteProgram(program);
}


}
