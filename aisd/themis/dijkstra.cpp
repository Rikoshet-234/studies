/*
 * Łukasz Hanuszczak
 * numer indeksu: 247952
 * grupa: KLO
 */

#include <cstdio>
#include <vector>
#include <set>

typedef unsigned int uint;
typedef uint vertex;

struct edge {
	vertex u;
	uint cost;

	edge(vertex u, uint cost) : u(u), cost(cost)
	{
	}
};

#define MAX_N 7000
#define MAX_M 300000
#define INF 999999999

std::vector<edge> edges[MAX_N + 1];
uint cost[MAX_N + 1];

struct cost_comp {
	bool operator () (const vertex &a, const vertex &b) const
	{
		return cost[a] != cost[b] ? cost[a] < cost[b] : a < b;
	}
};

int main()
{
	uint n, m;
	scanf("%u %u", &n, &m);

	for (uint i = 0; i < m; i++) {
		vertex a, b;
		uint c;
		scanf("%u %u %u", &a, &b, &c);

		edges[a].push_back(edge(b, c));
		edges[b].push_back(edge(a, c));
	}

	/* Na początku wszystkie koszty są nieskończone. */
	for (vertex v = 1; v <= n; v++) {
		cost[v] = +INF;
	}

	std::set<vertex, cost_comp> queue;
	cost[1] = 0;
	queue.insert(1);

	while (not queue.empty()) {
		/* Ściągamy z kolejki element o najmniejszym koszcie. */
		const vertex v = *queue.begin();
		queue.erase(queue.begin());

		for (uint i = 0; i < edges[v].size(); i++) {
			const edge e = edges[v][i];
			if (cost[e.u] > cost[v] + e.cost) {
				/* Aktualizacja wartości w kolejce. */
				std::set<vertex, cost_comp>::iterator it = queue.find(e.u);
				if (it != queue.end()) {
					queue.erase(it);
				}

				cost[e.u] = cost[v] + e.cost;
				queue.insert(e.u);
			}
		}
	}

	for (vertex v = 2; v <= n; v++) {
		printf("%u ", cost[v]);
	}
	printf("\n");

	return 0;
}
