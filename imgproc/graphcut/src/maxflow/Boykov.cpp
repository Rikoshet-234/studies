#include <algorithm>
#include <limits>

#include "../Log.hpp"
#include "Boykov.hpp"


#define INF (std::numeric_limits<float>::max())
#define ROOT (std::numeric_limits<size_t>::max())


Boykov::Boykov(Network &G, node_t s, node_t t)
: m_tree(G.size()),
  m_is_active(G.size()), m_is_orphan(G.size()),
  m_pred(G.size()), m_pred_arc_id(G.size()), m_flow(G.size())
{
	Log << "Running Boykov algorithm..." << std::endl;

	std::fill(m_pred.begin(), m_pred.end(), ROOT);
	std::fill(std::begin(m_is_active), std::end(m_is_active), false);
	std::fill(std::begin(m_is_orphan), std::end(m_is_orphan), false);
	std::fill(std::begin(m_tree), std::end(m_tree), Tree::NONE);

	m_tree[s] = Tree::S;
	m_tree[t] = Tree::T;
	m_flow[s] = INF;
	m_flow[t] = INF;
	activate(s);
	activate(t);

	m_iters = 0;
	res_arc_t *joint;
	while ((joint = grow(G)) != nullptr) {
		m_iters++;
		augment(G, s, t, *joint);
		adopt(G);
	}
}


res_arc_t *Boykov::grow(Network &G)
{
	while (!m_active.empty()) {
		node_t v = m_active.front();
		if (!m_is_active[v]) {
			m_active.pop();
			continue;
		}

		for (size_t i = 0; i < G.arcs(v).size(); i++) {
			auto &arc = G.arcs(v)[i];
			auto &darc = tree_arc(G, v, arc);
			if (darc.residual_capacity() <= EPSILON) {
				continue;
			}

			node_t u = arc.dst;
			if (m_tree[u] == Tree::NONE) {
				m_tree[u] = m_tree[v];
				m_pred[u] = v;
				m_pred_arc_id[u] = i;
				m_flow[u] = std::min(darc.residual_capacity(), m_flow[v]);
				activate(u);
			} else if (m_tree[u] != m_tree[v]) {
				return &darc;
			}
		}
		m_active.pop();
	}

	return nullptr;
}


void Boykov::augment(Network &G, node_t s, node_t t, res_arc_t &joint)
{
	node_t jv = G.inverted(joint).dst;
	node_t ju = joint.dst;
	node_t u, v;

	float flow = joint.residual_capacity();
	u = jv;
	while (u != s) {
		v = m_pred[u];
		// std::cerr << "looking up " << u << ", pred: " << v << "\n";
		flow = std::min(G.arcs(v)[m_pred_arc_id[u]].residual_capacity(), flow);
		u = v;
	}
	v = ju;
	while (v != t) {
		u = m_pred[v];
		// std::cerr << "looking up " << v << ", pred: " << u << "\n";
		flow = std::min(G.inverted(G.arcs(u)[m_pred_arc_id[v]]).residual_capacity(), flow);
		v = u;
	}

	Log << "Found augmenting path #" << m_iters
	    << " of flow " << flow << std::endl;


	// Agument joint.
	joint.flow += flow;
	G.inverted(joint).flow -= flow;
	// std::cerr << "! joint " << jv << " -> " << ju << "\n";

	// Augment S-tree.
	u = jv;
	while (u != s) {
		v = m_pred[u];
		// std::cerr << "> flowing " << v << " -> " << u << "\n";

		res_arc_t &arc = G.arcs(v)[m_pred_arc_id[u]];
		arc.flow += flow;
		G.inverted(arc).flow -= flow;

		if (arc.residual_capacity() <= EPSILON) {
			m_pred[u] = ROOT;
			orphanate(u);
		}

		u = v;
	}

	// Agument T-tree.
	v = ju;
	while (v != t) {
		u = m_pred[v];
		// std::cerr << "< flowing " << v << " -> " << u << "\n";

		res_arc_t &arc = G.arcs(u)[m_pred_arc_id[v]];
		G.inverted(arc).flow += flow;
		arc.flow -= flow;

		if (G.inverted(arc).residual_capacity() <= EPSILON) {
			m_pred[v] = ROOT;
			orphanate(v);
		}

		v = u;
	}
	std::cerr << ".";
}


void Boykov::adopt(Network &G)
{
	// std::cerr << "adopting\n" << G; 
	while (!m_orphans.empty()) {
		node_t v = m_orphans.front();
		// std::cerr << "processing oprhan " << v << "\n";
		m_orphans.pop();
		process(G, v);
	}
}


void Boykov::process(Network &G, node_t v)
{
	for (size_t i = 0; i < G.arcs(v).size(); i++) {
		auto &arc = G.arcs(v)[i];
		auto &darc = G.inverted(tree_arc(G, v, arc));
		node_t u = arc.dst;

		if (m_tree[v] == m_tree[u] &&
		    !m_is_orphan[u] &&
		    darc.residual_capacity() > EPSILON)
		{
			// std::cerr << "found parent for " << v << ": " << u << "\n";
			// std::cerr << "darc.dst = " << darc.dst << ", cap = " << darc.residual_capacity() << "\n";
			m_pred[v] = u;
			m_pred_arc_id[v] = i;
			m_flow[v] = std::min(darc.residual_capacity(), m_flow[u]);
			return;
		}
	}
	// std::cerr << "no parent for " << v << "\n";

	for (size_t i = 0; i < G.arcs(v).size(); i++) {
		auto &arc = G.arcs(v)[i];
		auto &darc = G.inverted(tree_arc(G, v, arc));
		node_t u = arc.dst;

		if (m_tree[v] != m_tree[u]) {
			continue;
		}

		if (darc.residual_capacity() > EPSILON) {
			activate(u);
			if (m_pred[u] == v) {
				orphanate(u);
				m_pred[u] = ROOT;
			}
		}
	}

	m_tree[v] = Tree::NONE;
	m_is_active[v] = false;
}
