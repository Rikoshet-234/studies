#include <algorithm>
#include <limits>

#include "../Log.hpp"
#include "Boykov2.hpp"


#define INF (std::numeric_limits<float>::max())
#define ROOT (std::numeric_limits<size_t>::max())


Boykov2::Boykov2(Network &G, node_t s, node_t t)
: m_tree(G.size()),
  m_pred(G.size()), m_pred_arc_id(G.size())
{
	Log << "Running Boykov algorithm..." << std::endl;

	std::fill(m_pred.begin(), m_pred.end(), ROOT);
	std::fill(std::begin(m_tree), std::end(m_tree), Tree::NONE);

	m_tree[s] = Tree::S;
	m_tree[t] = Tree::T;
	m_active.insert(s);
	m_active.insert(t);

	m_iters = 0;
	res_arc_t *joint;
	while ((joint = grow(G)) != nullptr) {
		m_iters++;
		augment(G, s, t, *joint);
		adopt(G, s, t);
	}
}


res_arc_t *Boykov2::grow(Network &G)
{
	while (!m_active.empty()) {
		node_t v = *m_active.begin();

		for (size_t i = 0; i < G.arcs(v).size(); i++) {
			auto &arc = G.arcs(v)[i];
			auto &darc = tree_arc(G, v, arc);
			if (darc.residual_capacity() <= Network::EPSILON) {
				continue;
			}

			node_t u = arc.dst;
			if (m_tree[u] == Tree::NONE) {
				m_tree[u] = m_tree[v];
				m_pred[u] = v;
				m_pred_arc_id[u] = i;
				m_active.insert(u);
			} else if (m_tree[u] != m_tree[v]) {
				return &darc;
			}
		}
		m_active.erase(m_active.begin());
	}

	return nullptr;
}


void Boykov2::augment(Network &G, node_t s, node_t t, res_arc_t &joint)
{
	node_t jv = G.inverted(joint).dst;
	node_t ju = joint.dst;
	node_t u, v;

	// Backtract trees to obtain minimum capacity on a path.
	float flow = joint.residual_capacity();

	u = jv;
	while (u != s) {
		v = m_pred[u];
		flow = std::min(G.arcs(v)[m_pred_arc_id[u]].residual_capacity(), flow);
		u = v;
	}

	v = ju;
	while (v != t) {
		u = m_pred[v];
		flow = std::min(G.inverted(G.arcs(u)[m_pred_arc_id[v]]).residual_capacity(), flow);
		v = u;
	}

	Log << "Found augmenting path #" << m_iters
	    << " of flow " << flow << std::endl;

	// Agument joint.
	joint.flow += flow;
	G.inverted(joint).flow -= flow;

	// Augment S-tree.
	u = jv;
	while (u != s) {
		v = m_pred[u];

		res_arc_t &arc = G.arcs(v)[m_pred_arc_id[u]];
		arc.flow += flow;
		G.inverted(arc).flow -= flow;

		if (arc.residual_capacity() <= Network::EPSILON) {
			m_orphans.push(u);
			m_pred[u] = ROOT;
		}

		u = v;
	}

	// Agument T-tree.
	v = ju;
	while (v != t) {
		u = m_pred[v];

		res_arc_t &arc = G.arcs(u)[m_pred_arc_id[v]];
		G.inverted(arc).flow += flow;
		arc.flow -= flow;

		if (G.inverted(arc).residual_capacity() <= Network::EPSILON) {
			m_orphans.push(v);
			m_pred[v] = ROOT;
		}

		v = u;
	}
}


void Boykov2::adopt(Network &G, node_t s, node_t t)
{ 
	while (!m_orphans.empty()) {
		node_t v = m_orphans.front();
		m_orphans.pop();
		process(G, v, s, t);
	}
}


void Boykov2::process(Network &G, node_t v, node_t s, node_t t)
{
	(void)s;
	(void)t;

	for (size_t i = 0; i < G.arcs(v).size(); i++) {
		auto &arc = G.arcs(v)[i];
		node_t u = arc.dst;

		if (m_pred[u] == v) {
			m_pred[u] = ROOT;
			m_orphans.push(u);
		}

		// auto &darc = G.inverted(tree_arc(G, v, arc));
		// node_t uorigin = origin(G, u);
		// if (m_tree[v] == m_tree[u] &&
		// 	(uorigin == s || uorigin == t) &&
		//     darc.residual_capacity() > Network::EPSILON)
		// {
		// 	m_pred[v] = u;
		// 	m_pred_arc_id[v] = arc.inverted_id;
		// 	return;
		// }
	}

	// for (size_t i = 0; i < G.arcs(v).size(); i++) {
	// 	auto &arc = G.arcs(v)[i];
	// 	auto &darc = G.inverted(tree_arc(G, v, arc));
	// 	node_t u = arc.dst;

	// 	if (m_tree[v] != m_tree[u]) {
	// 		continue;
	// 	}

	// 	if (darc.residual_capacity() > Network::EPSILON) {
	// 		m_active.insert(u);
	// 		if (m_pred[u] == v) {
	// 			m_orphans.push(u);
	// 			m_pred[u] = ROOT;
	// 		}
	// 	}
	// }

	m_tree[v] = Tree::NONE;

	auto it = m_active.find(v);
	if (it != std::end(m_active)) {
		m_active.erase(it);
	}
}


node_t Boykov2::origin(Network &G, node_t v) const
{
	node_t u = m_pred[v];
	while (u != ROOT) {
		auto &darc = tree_arc(G, v, G.arcs(u)[m_pred_arc_id[v]]);
		if (darc.residual_capacity() <= Network::EPSILON) {
			return ROOT;
		}

		v = u;
		u = m_pred[v];
	}
	return v;
}
