#include <cstdio>
#include <vector>
#include <algorithm>

typedef unsigned int uint;
typedef uint vertex;

#define MAX_N 100000
#define ROOT 1

std::vector<vertex> tree[MAX_N + 1];
uint with[MAX_N + 1];
uint without[MAX_N + 1];

void dfs(vertex v, vertex p)
{
	with[v] = 1;
	without[v] = 0;
	for (uint i = 0; i < tree[v].size(); i++) {
		const vertex &u = tree[v][i];
		
		if (u == p) {
			continue;
		}
		dfs(u, v);

		/* Nie bierzemy korzenia, to musimy wziąć syna. */
		without[v] += with[u];

		/* Bierzemy korzeń to możemy, ale nie musimy, brać syna. */
		with[v] += std::min(without[u], with[u]);
	}
}

int main()
{
	uint n;
	scanf("%u", &n);

	for (uint i = 1; i < n; i++) {
		vertex a, b;
		scanf("%u %u", &a, &b);
		
		tree[a].push_back(b);
		tree[b].push_back(a);
	}

	dfs(ROOT, 0);

	printf("%u\n", std::min(with[ROOT], without[ROOT]));;

	return 0;
}
