#include <gl/Shader.hpp>

#include <vector>
#include <sstream>
#include <fstream>
#include <stdexcept>


namespace gl {


Shader::Shader(shader_type type, std::istream &is)
{
	init(type, is);
}


Shader::Shader(shader_type type, const char *filename)
{
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::ostringstream message;
		message << "failed to open given shader file" << " "
		        << "(\"" << filename << "\")";
		throw std::invalid_argument(message.str());
	}

	init(type, file);
}


Shader::~Shader()
{
	glDeleteShader(shader);
}


Shader::Shader(Shader &&other)
{
	this->swap(other);
}


Shader &Shader::operator=(Shader &&other)
{
	this->swap(other);
	return *this;
}


void Shader::swap(Shader &other)
{
	using std::swap;

	swap(shader, other.shader);
}



void Shader::init(shader_type type, std::istream &is)
{
	std::string source;

	char c;
	while (is && (c = is.get())) {
		source += c;
	}

	switch (type) {
	case VertexShader:
		shader = glCreateShader(GL_VERTEX_SHADER);
		break;
	case FragmentShader:
		shader = glCreateShader(GL_FRAGMENT_SHADER);
		break;
	}

	auto raw = source.c_str();
	glShaderSource(shader, 1, &raw, nullptr);
	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_TRUE) {
		return;
	}

	GLint logLength;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

	std::vector<char> logMessage(logLength + 1);
	logMessage[logLength] = 0;
	
	glGetShaderInfoLog(shader, logLength, nullptr, logMessage.data());

	std::ostringstream message;
	message << "failed to compile shader" << "\n" << logMessage.data();
	throw std::runtime_error(message.str());
}


}
