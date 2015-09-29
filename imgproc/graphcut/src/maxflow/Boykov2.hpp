#ifndef MAXFLOW_BOYKOV2_HPP
#define MAXFLOW_BOYKOV2_HPP

#include <vector>
#include <queue>
#include <set>
using std::size_t;

#include "../Graph.hpp"
#include "../Network.hpp"


class Boykov2 {
public:
	static void execute(Network &G, node_t s, node_t t);

private:
	Boykov2(Network &G, node_t s, node_t t);

	enum class Tree { NONE, S, T };
	std::vector<Tree> m_tree;

	std::set<node_t> m_active;
	std::queue<node_t> m_orphans;

	std::vector<node_t> m_pred;
	std::vector<size_t> m_pred_arc_id;
	std::vector<float> m_flow;

	uint64_t m_iters;

	res_arc_t *grow(Network &G);
	void augment(Network &G, node_t s, node_t t, res_arc_t &joint);
	void adopt(Network &G, node_t s, node_t t);
	void process(Network &G, node_t v, node_t s, node_t t);

	res_arc_t &tree_arc(Network &G, node_t v, res_arc_t &arc) const;

	node_t origin(Network &G, node_t v) const;
};


inline void Boykov2::execute(Network &G, node_t s, node_t t)
{
	Boykov2 executor(G, s, t);
}


inline res_arc_t &Boykov2::tree_arc(Network &G, node_t v, res_arc_t &arc) const
{
	switch (m_tree[v]) {
	case Tree::S:
		return arc;
	case Tree::T:
		return G.inverted(arc);
	case Tree::NONE:
		// Not possible.
		return arc;
	}
}


#endif
