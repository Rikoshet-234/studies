#ifndef IMAGE_HPP
#define IMAGE_HPP


#include <vector>
using std::size_t;

#include "Self.hpp"


template<typename T>
class Image : public Self<Image<T>> {
public:
	explicit Image(size_t width, size_t height);

	size_t size() const {
		return m_data.size();
	}

	size_t width() const {
		return m_width;
	}

	size_t height() const {
		return m_height;
	}

	const T &operator()(size_t x, size_t y) const;
	T &operator()(size_t x, size_t y);

	const std::vector<T> &data() const;
	std::vector<T> &data();

private:
	size_t m_width, m_height;
	std::vector<T> m_data;
};


template<typename T>
Image<T>::Image(size_t width, size_t height)
: m_width(width), m_height(height), m_data(width * height)
{
}


template<typename T>
inline const T &Image<T>::operator()(size_t x, size_t y) const
{
	return m_data[y * m_width + x];
}


template<typename T>
inline T &Image<T>::operator()(size_t x, size_t y)
{
	return const_cast<T &>(this->self()(x, y));
}


template<typename T>
inline const std::vector<T> &Image<T>::data() const
{
	return m_data;
}


template<typename T>
inline std::vector<T> &Image<T>::data()
{
	return const_cast<std::vector<T> &>(this->self().data());
}


#endif
