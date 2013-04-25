/*
 * Łukasz Hanuszczak
 * numer indeksu: 247952
 * grupa: KLO
 */

#include <cstdio>
#include <queue>

typedef unsigned int uint;

struct coord {
	uint x, y;

	coord() : x(0), y(0)
	{
	}

	coord(const uint &x, const uint &y) : x(x), y(y)
	{
	}
};

#define MAX_M 2000
#define MAX_N 2000
#define INF ('I' * 'N' * 'F')
#define WALL ('W' * 'A' * 'L' * 'L')

uint distance[MAX_M + 2][MAX_N + 2];

int main()
{
	/* Wczytywanie danych. */
	uint m, n;
	scanf("%u %u\n", &m, &n);

	coord p, s;

	char *row = new char [m + 1];
	for (uint y = 1; y <= n; y++) {
		fgets(row, m + 2, stdin);
		for (uint x = 1; x <= m; x++) {
			distance[x][y] = +INF;

			if (row[x - 1] == 'x') {
				distance[x][y] = WALL;
			} else if (row[x - 1] == 'P') {
				p = coord(x, y);
			} else if (row[x - 1] == 'S') {
				s = coord(x, y);
			}
		}
	}

	/* Ustaw ściany na krańcach, aby nie wyjść poza mapę. */
	for (uint x = 1; x <= m; x++) {
		distance[x][0] = distance[x][n + 1] = WALL;
	}
	for (uint y = 1; y <= n; y++) {
		distance[0][y] = distance[m + 1][y] = WALL;
	}

	/* Najzwyklejszy w świecie BFS. */
	std::queue<coord> q;
	q.push(p);
	distance[p.x][p.y] = 0;

	while (not q.empty()) {
		const coord c = q.front();
		const uint d = distance[c.x][c.y] + 1;

		if (distance[c.x - 1][c.y] == +INF) {
			/* Idź w lewo. */
			q.push(coord(c.x - 1, c.y));
			distance[c.x - 1][c.y] = d;
		}

		if (distance[c.x + 1][c.y] == +INF) {
			/* Idź w prawo. */
		 	q.push(coord(c.x + 1, c.y));
			distance[c.x + 1][c.y] = d;
		}

		if (distance[c.x][c.y - 1] == +INF) {
			/* Idź do dołu. */
			q.push(coord(c.x, c.y - 1));
			distance[c.x][c.y - 1] = d;
		}

		if (distance[c.x][c.y + 1] == +INF) {
			/* Idź do góry. */
			q.push(coord(c.x, c.y + 1));
			distance[c.x][c.y + 1] = d;
		}

		q.pop();
	}

	/* Wypisz odpowiedź, czyli odległość od skarbu. */
	if (distance[s.x][s.y] == +INF) {
		printf("NIE\n");
	} else {
		printf("%u\n", distance[s.x][s.y]);
	}

	return 0;
}
