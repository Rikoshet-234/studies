#ifndef NETWORK_HPP
#define NETWORK_HPP


#include <vector>
#include <ostream>
using std::size_t;

#include "Self.hpp"
#include "Graph.hpp"


struct res_arc_t : public arc_t {
	float flow;
	size_t inverted_id;

	float residual_capacity() const {
		return capacity - flow;
	}
};


class Network : public Self<Network> {
public:
	explicit Network(const Graph &G);

	const std::vector<res_arc_t> &arcs(node_t v) const;
	std::vector<res_arc_t> &arcs(node_t v);

	const res_arc_t &inverted(res_arc_t arc) const;
	res_arc_t &inverted(res_arc_t arc);

	size_t size() const {
		return m_arcs.size();
	}

	static constexpr float EPSILON = 0.001f;
private:
	std::vector<std::vector<res_arc_t>> m_arcs;

	friend std::ostream &operator<<(std::ostream &ostream, const Network &G);
};


inline const std::vector<res_arc_t> &Network::arcs(node_t v) const
{
	return m_arcs[v];
}


inline std::vector<res_arc_t> &Network::arcs(node_t v)
{
	return const_cast<std::vector<res_arc_t> &>(self().arcs(v));
}


inline const res_arc_t &Network::inverted(res_arc_t arc) const
{
	return m_arcs[arc.dst][arc.inverted_id];
}


inline res_arc_t &Network::inverted(res_arc_t arc)
{
	return const_cast<res_arc_t &>(self().inverted(arc));
}


#endif
