/*
 * Łukasz Hanuszczak
 * numer indeksu: 247952
 * grupa: KLO
 */

#include <cstdio>

typedef unsigned int uint;
typedef uint vertex;

struct edge {
	vertex a, b;
	int weight;

	edge() : a(0), b(0), weight(0)
	{
	}

	edge(const vertex &a, const vertex &b, const int &weight)
	: a(a), b(b), weight(weight)
	{
	}
};

#define MAX_N 500
#define MAX_M (500 * 500)
#define INF (999999999)

edge edges[MAX_M + 1];
int distance[MAX_N + 1];

int main()
{
	uint n, m;
	vertex s;
	scanf("%u %u %u", &n, &m, &s);

	for (uint i = 0; i < m; i++) {
		vertex a, b;
		int c;
		scanf("%u %u %d", &a, &b, &c);

		edges[i] = edge(a, b, c);
	}

	/* Ustawianie początkowych wartości. */
	for (vertex v = 0; v <= n; v++) {
		distance[v] = +INF;
	}
	distance[s] = 0;

	/* Bellman-Ford w całej swojej okazałości. */
	for (uint i = 0; i < n; i++) {
		for (uint j = 0; j < m; j++) {
			const edge e = edges[j];

			if (distance[e.a] == +INF) {
				continue;
			}
			if (distance[e.b] > distance[e.a] + e.weight) {
				distance[e.b] = distance[e.a] + e.weight;
			}
		}
	}

	/* Sprawdź czy przypadkiem nie ma ujemnego cyklu. */
	for (uint j = 0; j < m; j++) {
		const edge e = edges[j];

		if (distance[e.a] == +INF) {
			continue;
		}
		if (distance[e.b] > distance[e.a] + e.weight) {
			printf("NIE\n");
			return 0;
		}
	}

	for (vertex v = 0; v <= n; v++) {
		if (v != s and distance[v] != +INF) {
			printf("%u %d\n", v, distance[v]);
		}
	}

	return 0;
}
