/*
 * Łukasz Hanuszczak
 * numer indeksu: 247952
 * grupa: KLO
 */

#include <cstdio>
#include <algorithm>

typedef unsigned int uint;

struct point {
	int x, y;
};

struct event {
	int x; /* Wsółrzędna na osi OX rozpatrywanego wydarzenia. */
	int bottom, top; /* Współrzędne na osi OY fragmentu prostokąta. */
	int w; /* Wielkość opadu: dodatnia jeżeli początek prostokąta. */

	bool operator <(const event &other) const
	{
		return (x != other.x) ? (x < other.x) : (w > other.w);
	}
};

/**
 * Implementacja drzewa przedziałowego pozwalająca wstawiać przedziały postaci
 * [a, b] gdzie 0 <= a < b <= n - 1, gdzie n to ilość liści drzewa. Struktura
 * umożliwia szybkie sprawdzenie maksymalnej wartości pewnego punktu w całym
 * drzewie (wartość tą można odczytać z korzenia).
 *
 * Sama struktura drzewa została oparta o implementację Rafała Nowaka (więcej
 * na stronie http://rafalnowak.pl/wiki/index.php?title=Drzewo_przedziałowe),
 * zaś pomysł na nierekurencyjne zapytania powstał na podstawie artykułu
 * z serii "Wykłady z Algorytmiki Stosowanej" (więcej na stronie
 * http://was.zaa.mimuw.edu.pl/?q=node/9).
 */
template <typename T>
class range_tree {
private:
	typedef uint vertex;

	/* Funkcje pomocnicze. */
	static const vertex root = 1;
	static inline vertex father(const vertex &v) { return v / 2; }
	static inline vertex lchild(const vertex &v) { return 2 * v; }
	static inline vertex rchild(const vertex &v) { return 2 * v + 1; }
	inline vertex leaf(const vertex &v) const { return leaves + v; }

	size_t leaves; /* Ilość liści w drzewie. */

	T *vload; /* Obciążenie wierzchołka. */
	T *vmax; /* Maksymalna suma w poddrzewie zawieszonym w wierzchołku. */

	inline void update_max(const vertex &v)
	{
		vmax[v] = std::max(vmax[lchild(v)], vmax[rchild(v)]) + vload[v];
	}

public:
	/**
	 * Konstruuje nowe drzewo przedziałowe.
	 *
	 * @param size Ilość fizycznych liści w drzewie.
	 */
	range_tree(const size_t &size) : leaves(1)
	{
		/* Obliczamy liczbę liści i wysokość drzewa. */
		while (leaves < size) {
			leaves *= 2;
		}

		vload = new T [2 * leaves + 1];
		vmax = new T [2 * leaves + 1];
	}

	~range_tree()
	{
		delete[] vload;
		delete[] vmax;
	}
	
	/**
	 * Wstawia do drzewa nowy przedział o zadanej wartości.
	 *
	 * @param beg Początek wstawianego przedziału.
	 * @param end Koniec wstawianego przedziału.
	 * @param value Wartość przedziału.
	 * @return Maksymalna wartość pewnego punktu w drzewie po aktualizacji.
	 */
	T query(const uint &beg, const uint &end, const T &value)
	{
		vertex a = leaf(beg);
		vertex b = leaf(end);

		vload[a] += value;
		vmax[a] += value;
		vload[b] += (a != b) ? value : 0;
		vmax[b] += (a != b) ? value : 0;

		while (father(a) != father(b)) {
			if (a % 2 == 0) {
				vload[a + 1] += value;
				vmax[a + 1] += value;
			}
			if (b % 2 == 1) {
				vload[b - 1] += value;
				vmax[b - 1] += value;
			}

			update_max(a = father(a));
			update_max(b = father(b));
		}

		while (a != root) {
			update_max(a = father(a));
		}

		return vmax[root];
	}

};

#define INF 20000000 
#define MAX_N 100000
event events[2 * MAX_N + 1];
int *aux[4 * MAX_N + 1];

bool cmp_aux(const int *a, const int *b) { return *a < *b; }

int main()
{
	uint n;
	scanf("%u", &n);

	/* Budujemy tablicę wydarzeń na podstawie wczytanych prostokątów. */
	const uint m = 2 * n;
	for (uint i = 0, j = 0; i < m;) {
		point bl, tr;
		scanf("%d %d", &bl.x, &bl.y);
		scanf("%d %d", &tr.x, &tr.y);

		int w;
		scanf("%d", &w);

		events[i++] = (event){ bl.x, bl.y, tr.y, +w };
		aux[j++] = &events[i - 1].bottom;
		aux[j++] = &events[i - 1].top;
		events[i++] = (event){ tr.x, bl.y, tr.y, -w };
		aux[j++] = &events[i - 1].bottom;
		aux[j++] = &events[i - 1].top;
	}

	/* Kompresujemy wydarzenia. */
	std::sort(aux, aux + 2 * m, cmp_aux);
	int last = *aux[0];
	*aux[0] = 0;
	for (uint i = 1; i < 2 * m; i++) {
		if (*aux[i] == last) {
			last = *aux[i];
			*aux[i] = *aux[i - 1];
		} else {
			last = *aux[i];
			*aux[i] = *aux[i - 1] + 1;
		}
	}

	int max_sum = 0; /* Maksymalny sumaryczny opad nad jednym prostokątem. */
	
	/* Zamiatamy od lewej do prawej, zbierając wynik. */
	std::sort(events, events + m);
	range_tree<int> tree(m);
	for (uint i = 0; i < m; i++) {
		const event &e = events[i];
		max_sum = std::max(max_sum, tree.query(e.bottom, e.top, e.w));
	}

	printf("%d\n", max_sum);

	return 0;
}
