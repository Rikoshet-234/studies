#ifndef MAXFLOW_EDMONDS_KARP_HPP
#define MAXFLOW_EDMONDS_KARP_HPP


#include <vector>
using std::size_t;

#include "../Graph.hpp"
#include "../Network.hpp"


class EdmondsKarp {
public:
	static void execute(Network &G, node_t s, node_t t);

private:

	EdmondsKarp(Network &G, node_t s, node_t t);

	bool bfs(const Network &G, node_t s, node_t t);
	void augment(Network &G, node_t s, node_t t);

	std::vector<float> m_flow;
	std::vector<node_t> m_pred;
	std::vector<size_t> m_pred_arc_id;

	uint32_t m_iters;
};


inline void EdmondsKarp::execute(Network &G, node_t s, node_t t)
{
	EdmondsKarp executor(G, s, t);
}


#endif
