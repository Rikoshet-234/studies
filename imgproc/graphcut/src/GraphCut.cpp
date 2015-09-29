#include <algorithm>
#include <queue>
#include <cmath>

#include "Log.hpp"
#include "GraphCut.hpp"
#include "Network.hpp"
#include "maxflow/EdmondsKarp.hpp"
#include "maxflow/Boykov.hpp"
#include "maxflow/Boykov2.hpp"


#define OBJ_HIST_FILEPATH "obj.hist"
#define BKG_HIST_FILEPATH "bkg.hist"


static void bfs(const Network &G, node_t s, node_t t,
                Image<Selection> &segmentation);


GraphCut::GraphCut(const Image<float> &picture,
                   const Image<Selection> &selection,
                   Algorithm algorithm, float lambda,
                   Image<Selection> &segmentation)
: m_picture(picture), m_selection(selection), m_lambda(lambda)
{
	build_histograms();

	Log << "Dumping object histogram to \"" << OBJ_HIST_FILEPATH << "\"\n";
	{
		std::ofstream file(OBJ_HIST_FILEPATH);
		m_PO.dump(file);
	}
	Log << "Dumping background histogram to \"" << BKG_HIST_FILEPATH << "\"\n";
	{
		std::ofstream file(BKG_HIST_FILEPATH);
		m_PB.dump(file);
	}

	Log << "Initializing image graph..." << std::endl;
	Graph G(width() * height() + 2);
	float K = build_nlinks(G) + 1.0f;
	build_tlinks(K, G);

	Log << "Creating residual network..." << std::endl;
	Network G1(G);

	switch (algorithm) {
	case Algorithm::EDMONDS:
		EdmondsKarp::execute(G1, S(), T());
		break;
	case Algorithm::BOYKOV:
		Boykov::execute(G1, S(), T());
		break;
	case Algorithm::BOYKOV2:
		Boykov2::execute(G1, S(), T());
		break;
	}
	bfs(G1, S(), T(), segmentation);

	Log << "Segmentation done!" << std::endl;
}


Image<Selection> GraphCut::execute(const Image<float> &picture,
                                   const Image<Selection> &selection,
                                   Algorithm algorithm, float lambda)
{
	Image<Selection> result(picture.width(), picture.height());
	GraphCut executor(picture, selection, algorithm, lambda, result);
	return std::move(result);
}


void GraphCut::build_histograms()
{
	Log << "Building histograms..." << std::endl;

	for (size_t y = 0; y < height(); y++) {
		for (size_t x = 0; x < width(); x++) {
			build_histograms(x, y);
		}
	}
}


void GraphCut::build_histograms(size_t x, size_t y)
{
	switch (m_selection(x, y)) {
	case Selection::OBJECT:
		m_PO.put(m_picture(x, y));
		break;
	case Selection::BACKGROUND:
		m_PB.put(m_picture(x, y));
		break;
	case Selection::NONE:
		break;
	}
}


float GraphCut::build_nlinks(Graph &G) const
{
	Log << "Creating n-links..." << std::endl;

	float max = 0.0f;
	for (size_t y = 0; y < height(); y++) {
		for (size_t x = 0; x < width(); x++) {
			max = std::max(max, add_nlinks(G, x, y));
		}
	}
	return max;
}


float GraphCut::add_nlinks(Graph &G, size_t x, size_t y) const
{
	float total = 0.0f;
	if (x > 0) {
		total += add_nlink(G, x, y, x - 1, y);
	}
	if (y > 0) {
		total += add_nlink(G, x, y, x, y - 1);
	}
	if (x < width() - 1) {
		total += add_nlink(G, x, y, x + 1, y);
	}
	if (y < height() - 1) {
		total += add_nlink(G, x, y, x, y + 1);
	}
	return total;
}


float GraphCut::add_nlink(Graph &G,
                          size_t vx, size_t vy, size_t ux, size_t uy) const
{
	float capacity = B(vx, vy, ux, uy);
	G.add_arc(node(vx, vy), node(ux, uy), capacity);
	return capacity;
}


void GraphCut::build_tlinks(float K, Graph &G) const
{
	Log << "Creating t-links..." << std::endl;

	for (size_t y = 0; y < height(); y++) {
		for (size_t x = 0; x < width(); x++) {
			float scap = tlink_S_capacity(K, x, y);
			float tcap = tlink_T_capacity(K, x, y);

			G.add_arc(S(), node(x, y), scap);
			G.add_arc(node(x, y), T(), tcap);
		}
	}
}


float GraphCut::tlink_S_capacity(float K, size_t x, size_t y) const
{
	switch (m_selection(x, y)) {
	case Selection::OBJECT:
		return K;
	case Selection::BACKGROUND:
		return 0.0f;
	case Selection::NONE:
		return m_lambda * R(x, y, Selection::BACKGROUND);
	}
}


float GraphCut::tlink_T_capacity(float K, size_t x, size_t y) const
{
	switch (m_selection(x, y)) {
	case Selection::OBJECT:
		return 0.0f;
	case Selection::BACKGROUND:
		return K;
	case Selection::NONE:
		return m_lambda * R(x, y, Selection::OBJECT);
	}
}


static void bfs(const Network &G, node_t s, node_t t,
                Image<Selection> &segmentation)
{
	Log << "Retrieving minimum cut..." << std::endl;

	std::fill(std::begin(segmentation.data()), std::end(segmentation.data()),
	          Selection::BACKGROUND);

	std::queue<node_t> queue;
	queue.push(s);

	while (!queue.empty()) {
		node_t v = queue.front();
		queue.pop();

		for (const auto &arc : G.arcs(v)) {
			node_t u = arc.dst;

			if (u == s || u == t ||
				segmentation.data()[u] == Selection::OBJECT ||
			    arc.residual_capacity() <= Network::EPSILON)
			{
				continue;
			}

			segmentation.data()[u] = Selection::OBJECT;
			queue.push(u);
		}
	}
}
