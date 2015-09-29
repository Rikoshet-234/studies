#ifndef GRAPH_HPP
#define GRAPH_HPP


#include <vector>
#include <ostream>
using std::size_t;

#include "Self.hpp"


using node_t = size_t;


struct arc_t {
	node_t dst;
	float capacity;
};


class Graph : public Self<Graph> {
public:
	explicit Graph(size_t size);

	void add_arc(node_t v, node_t u, float capacity);
	void add_arc(node_t v, arc_t arc);

	const std::vector<arc_t> &arcs(node_t v) const;
	std::vector<arc_t> &arcs(node_t v);

	size_t size() const {
		return m_arcs.size();
	}

private:
	std::vector<std::vector<arc_t>> m_arcs;

	friend std::ostream &operator<<(std::ostream &ostream, const Graph &G);
};


inline void Graph::add_arc(node_t v, node_t u, float capacity)
{
	arc_t arc;
	arc.dst = u;
	arc.capacity = capacity;
	add_arc(v, arc);
}


inline const std::vector<arc_t> &Graph::arcs(node_t v) const
{
	return m_arcs[v];
}


inline std::vector<arc_t> &Graph::arcs(node_t v)
{
	return const_cast<std::vector<arc_t> &>(self().arcs(v));
}


#endif
