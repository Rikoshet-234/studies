#ifndef MAXFLOW_BOYKOV_HPP
#define MAXFLOW_BOYKOV_HPP

#include <vector>
#include <queue>
using std::size_t;

#include "../Graph.hpp"
#include "../Network.hpp"


class Boykov {
public:
	static void execute(Network &G, node_t s, node_t t);

	static constexpr float EPSILON = 0.001f;
private:
	Boykov(Network &G, node_t s, node_t t);

	enum class Tree { NONE, S, T };
	std::vector<Tree> m_tree;

	std::queue<node_t> m_active;
	std::queue<node_t> m_orphans;

	std::vector<bool> m_is_active;
	std::vector<bool> m_is_orphan;

	std::vector<node_t> m_pred;
	std::vector<size_t> m_pred_arc_id;
	std::vector<float> m_flow;

	uint64_t m_iters;

	res_arc_t *grow(Network &G);
	void augment(Network &G, node_t s, node_t t, res_arc_t &joint);
	void adopt(Network &G);
	void process(Network &G, node_t v);

	void activate(node_t v);
	void orphanate(node_t v);

	res_arc_t &tree_arc(Network &G, node_t v, res_arc_t &arc) const;
};


inline void Boykov::execute(Network &G, node_t s, node_t t)
{
	Boykov executor(G, s, t);
}


inline res_arc_t &Boykov::tree_arc(Network &G, node_t v, res_arc_t &arc) const
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


inline void Boykov::activate(node_t v)
{
	m_is_active[v] = true;
	m_active.push(v);
}


inline void Boykov::orphanate(node_t v)
{
	m_is_orphan[v] = true;
	m_orphans.push(v);
}


#endif
