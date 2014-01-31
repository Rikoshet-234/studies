#ifndef GL_ARRAY_BUFFER_H
#define GL_ARRAY_BUFFER_H


#include <iostream>
#include <vector>
#include <initializer_list>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>


namespace gl {


template <typename T>
class ArrayBuffer {
private:
	GLuint vbo;
	bool good = false;

	void init(const T *buffer, size_t size);
public:
	ArrayBuffer();
	ArrayBuffer(const T *buffer, size_t size);
	ArrayBuffer(std::initializer_list<T> buffer);
	ArrayBuffer(const std::vector<T> &buffer);
	~ArrayBuffer();

	ArrayBuffer & operator=(const std::vector<T> &buffer);
	ArrayBuffer & operator=(std::initializer_list<T> buffer);

	ArrayBuffer(ArrayBuffer &&other);
	ArrayBuffer(const ArrayBuffer &other) = delete;

	ArrayBuffer & operator=(ArrayBuffer &&other);
	ArrayBuffer & operator=(const ArrayBuffer &other) = delete;

	operator GLuint() const {
		return vbo;
	}

	operator bool() const {
		return good;
	}
};


template <typename T>
void ArrayBuffer<T>::init(const T *buffer, size_t size) {
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(T), buffer, GL_STATIC_DRAW);

	GLenum error = glGetError();
	
	if (error == GL_NO_ERROR) {
		good = true;
		return;
	}

	std::cerr << "Array buffer error (code: " << error << ").\n";
}


template <typename T>
ArrayBuffer<T>::ArrayBuffer() {
}


template <typename T>
ArrayBuffer<T>::ArrayBuffer(const T *buffer, size_t size) {
	init(buffer, size);
}


template <typename T>
ArrayBuffer<T>::ArrayBuffer(std::initializer_list<T> buffer) {
	init(std::begin(buffer), buffer.size());
}


template <typename T>
ArrayBuffer<T>::ArrayBuffer(const std::vector<T> &buffer) {
	init(buffer.data(), buffer.size());
}


template <typename T>
ArrayBuffer<T>::ArrayBuffer(ArrayBuffer &&other) : vbo(other.vbo), good(other.good) {
	good = false;
}


template <typename T>
ArrayBuffer<T> & ArrayBuffer<T>::operator=(ArrayBuffer &&other) {
	this->~ArrayBuffer(); // this instead ArrayBuffer because of clang bug.
	good = other.good;
	vbo = other.vbo;
	other.good = false;

	return (*this);
}


template <typename T>
ArrayBuffer<T> & ArrayBuffer<T>::operator=(std::initializer_list<T> buffer) {
	this->~ArrayBuffer(); // Same as above.
	init(std::begin(buffer), buffer.size());

	return (*this);
}


template <typename T>
ArrayBuffer<T> & ArrayBuffer<T>::operator=(const std::vector<T> &buffer) {
	this->~ArrayBuffer(); // Same as above.
	init(buffer.data(), buffer.size());

	return (*this);
}


template <typename T>
ArrayBuffer<T>::~ArrayBuffer() {
	if (good) {
		glDeleteBuffers(1, &vbo);
	}
}


}


#endif
