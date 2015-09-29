#include "Log.hpp"
#include "Network.hpp"


Network::Network(const Graph &G)
: m_arcs(G.size())
{
	for (node_t v = 0; v < G.size(); v++) {
		for (const arc_t &arc : G.arcs(v)) {
			node_t u = arc.dst;

			// For efficiency we omit zero-valued edges.
			if (arc.capacity <= EPSILON) {
				continue;
			}

			res_arc_t res_arc;
			res_arc.capacity = arc.capacity;

			res_arc.dst = u;
			res_arc.flow = 0.0f;
			res_arc.inverted_id = arcs(u).size();
			arcs(v).push_back(res_arc);

			res_arc.dst = v;
			res_arc.flow = arc.capacity;
			res_arc.inverted_id = arcs(v).size() - 1;
			arcs(u).push_back(res_arc);
		}
	}
}


std::ostream &operator<<(std::ostream &ostream, const Network &G)
{
	for (node_t v = 0; v < G.size(); v++) {
		ostream << v << ": ";
		for (const auto &arc : G.arcs(v)) {
			ostream << arc.dst
			        << " (" << arc.flow << "/" << arc.capacity << "), ";
		}
		ostream << "\n";
	}

	return ostream;
}
