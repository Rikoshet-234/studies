#ifndef HISTOGRAM_HPP
#define HISTOGRAM_HPP


#include <array>
#include <ostream>
#include <cmath>

#include "Image.hpp"


class Histogram {
public:
	explicit Histogram();
	explicit Histogram(const Image<float> &image);

	void put(float value);
	float operator()(float value) const;

	size_t count(float value) const {
		return m_buckets[bucket(value)];
	}

	void dump(std::ostream &ostream);

private:
	static constexpr size_t BUCKET_COUNT = 52;//256;

	size_t m_total;
	std::array<size_t, BUCKET_COUNT> m_buckets;

	size_t bucket(float value) const {
		return std::round(value * (BUCKET_COUNT - 1));
	}
};


inline void Histogram::put(float value)
{
	m_buckets[bucket(value)]++;
}


inline float Histogram::operator()(float value) const
{
	if (m_total == 0) {
		return 0.0f;
	}
	return static_cast<float>(count(value)) / m_total;
}


#endif
