#include "Graph.hpp"


Graph::Graph(size_t size)
: m_arcs(size)
{
}


void Graph::add_arc(node_t v, arc_t arc)
{
	m_arcs[v].push_back(arc);
}


std::ostream &operator<<(std::ostream &ostream, const Graph &G)
{
	for (node_t v = 0; v < G.size(); v++) {
		ostream << v << ": ";
		for (const auto &arc : G.arcs(v)) {
			ostream << arc.dst << " (" << arc.capacity << "), ";
		}
		ostream << "\n";
	}

	return ostream;
}
