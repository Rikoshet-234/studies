#include <queue>
#include <algorithm>

#include "../Log.hpp"
#include "EdmondsKarp.hpp"


#define INF (std::numeric_limits<float>::max())
#define NONE (std::numeric_limits<size_t>::max())


EdmondsKarp::EdmondsKarp(Network &G, node_t s, node_t t)
: m_flow(G.size()), m_pred(G.size()), m_pred_arc_id(G.size())
{
	Log << "Running Edmonds-Karp algorithm..." << std::endl;

	m_iters = 0;
	while (bfs(G, s, t)) {
		m_iters++;
		augment(G, s, t);
	}
}


bool EdmondsKarp::bfs(const Network &G, node_t s, node_t t)
{
	std::fill(std::begin(m_pred), std::end(m_pred), NONE);

	m_flow[s] = INF;
	m_pred[s] = s;

	std::queue<node_t> queue;
	queue.push(s);

	while (!queue.empty()) {
		node_t v = queue.front();
		queue.pop();

		for (size_t i = 0; i < G.arcs(v).size(); i++) {
			const auto &arc = G.arcs(v)[i];
			node_t u = arc.dst;

			if (m_pred[u] != NONE ||
			    arc.residual_capacity() <= Network::EPSILON)
			{
				continue;
			}

			m_flow[u] = std::min(m_flow[v], arc.residual_capacity());
			m_pred[u] = v;
			m_pred_arc_id[u] = i;
			queue.push(u);

			if (u == t) {
				return true;
			}
		}
	}

	return false;
}


void EdmondsKarp::augment(Network &G, node_t s, node_t t)
{
	float flow = m_flow[t];
	Log << "Found augmenting path #" << m_iters << " of flow " << flow << "\n";

	node_t u = t;
	while (u != s) {
		node_t v = m_pred[u];

		res_arc_t &arc = G.arcs(v)[m_pred_arc_id[u]];
		arc.flow += flow;
		G.inverted(arc).flow -= flow;

		u = v;
	}
}
