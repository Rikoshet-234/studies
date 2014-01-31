#include <gl/Shader.hpp>


namespace gl {


Shader::Shader()
{
}


Shader::Shader(shader_type type, std::istream &is)
{
	std::cout << "Compiling shader from input stream...\n";
	init(type, is);
}


Shader::Shader(Shader &&other) : shader(other.shader), good(other.good)
{
	other.good = false;
}


Shader::Shader(shader_type type, const char *filename)
{
	std::ifstream file(filename);

	if (!file) {
		std::cerr << "Error while opening \"" << filename << "\".\n";
		return;
	}

	std::cout << "Compiling shader \"" << filename << "\"...\n";
	init(type, file);
}


Shader & Shader::operator=(Shader &&other)
{
	this->~Shader();
	shader = other.shader;
	good = other.good;
	other.good = false;

	return (*this);
}


Shader::~Shader()
{
	if (good) {
		glDeleteShader(shader);
	}
}


void Shader::init(shader_type type, std::istream &is)
{
	std::string source;

	// We simply copy input stream to a source string.
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
	glShaderSource(shader, 1, &raw, NULL);
	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (status == GL_TRUE) {
		good = true;
		return;
	}

	// Something went wrong...
	GLint logLength;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

	std::unique_ptr<char> logMessage(new char[logLength + 1]);
	logMessage.get()[logLength] = 0;
	glGetShaderInfoLog(shader, logLength, NULL, logMessage.get());
	std::cerr << "shader error: " << logMessage.get() << "\n";	
}


}
