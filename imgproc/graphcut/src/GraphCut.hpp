#ifndef GRAPHCUT_HPP
#define GRAPHCUT_HPP


#include "Image.hpp"
#include "Histogram.hpp"
#include "Graph.hpp"


enum class Selection {
	OBJECT,
	BACKGROUND,
	NONE
};


enum class Algorithm {
	EDMONDS,
	BOYKOV,
	BOYKOV2
};


class GraphCut {
public:
	static Image<Selection> execute(const Image<float> &picture,
	                                const Image<Selection> &selection,
	                                Algorithm algorithm, float lambda);

private:
	static constexpr float SIGMA = 0.01f;

	const Image<float> &m_picture;
	const Image<Selection> &m_selection;

	float m_lambda;

	Histogram m_PO;
	Histogram m_PB;

	GraphCut(const Image<float> &picture, const Image<Selection> &selection,
	         Algorithm algorithm, float lambda,
	         Image<Selection> &segmentation);

	void build_histograms();
	void build_histograms(size_t x, size_t y);

	float build_nlinks(Graph &G) const;
	void build_tlinks(float K, Graph &G) const;

	float add_nlinks(Graph &G, size_t x, size_t y) const;
	float add_nlink(Graph &G,
	                size_t vx, size_t vy, size_t ux, size_t uy) const;

	float tlink_S_capacity(float K, size_t x, size_t y) const;
	float tlink_T_capacity(float K, size_t x, size_t y) const;

	size_t width() const {
		return m_picture.width();
	}

	size_t height() const {
		return m_picture.height();
	}

	node_t S() const {
		return width() * height() + 0;
	}

	node_t T() const {
		return width() * height() + 1;
	}

	node_t node(size_t x, size_t y) const {
		return y * width() + x;
	}

	float B(size_t vx, size_t vy, size_t ux, size_t uy) const;
	float R(size_t x, size_t y, Selection selection) const;
};


inline float GraphCut::B(size_t vx, size_t vy, size_t ux, size_t uy) const
{
	float dx = static_cast<float>(vx) - static_cast<float>(ux);
	float dy = static_cast<float>(vy) - static_cast<float>(uy);
	float i = m_picture(vx, vy) - m_picture(ux, uy);

	return std::exp(-(i * i) / (2.0f * SIGMA * SIGMA)) / std::hypot(dx, dy);
}


inline float GraphCut::R(size_t x, size_t y, Selection selection) const
{
	switch (selection) {
	case Selection::OBJECT:
		return -std::log(m_PO(m_picture(x, y)) + 0.01f);
	case Selection::BACKGROUND:
		return -std::log(m_PB(m_picture(x, y)) + 0.01f);
	case Selection::NONE:
		return 0.0f;
	}
}


#endif
