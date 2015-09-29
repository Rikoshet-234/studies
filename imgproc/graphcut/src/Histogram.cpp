#include "Histogram.hpp"
#include "Log.hpp"


Histogram::Histogram()
{
	std::fill(std::begin(m_buckets), std::end(m_buckets), 0);
}


Histogram::Histogram(const Image<float> &image)
: Histogram()
{
	for (auto &value : image.data()) {
		put(value);
	}
}


void Histogram::dump(std::ostream &ostream)
{
	for (auto bucket : m_buckets) {
		ostream << bucket << std::endl;
	}
}
