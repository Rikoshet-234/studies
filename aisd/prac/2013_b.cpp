/*
 * Łukasz Hanuszczak
 * numer indeksu: 247952
 * grupa: KLO
 */

#include <cstdio>
#include <cstdlib>

typedef unsigned int uint;
typedef unsigned long long uint64;

template<typename T>
class heap {
private:
	typedef uint vertex;

	const uint capacity;
	uint n;
	T* array;

	static vertex father(const vertex &v)
	{
		return v / 2;
	}

	static vertex lchild(const vertex &v)
	{
		return 2 * v;
	}
	
	static vertex rchild(const vertex &v)
	{
		return 2 * v + 1;
	}

	void slide_up(const vertex &v, const T &value)
	{
		/* Jesteśmy w korzeniu, nie ma gdzie wyżej jechać. */
		if (v == 1) {
			array[v] = value;
			return;
		}

		/* Jeżeli podstawienie wartości może zaburzyć porządek - rekurencja. */
		if (value < array[father(v)]) {
			array[v] = array[father(v)];
			slide_up(father(v), value);
		} else {
			array[v] = value;
		}
	}

	void slide_down(const vertex &v, const T &value)
	{
		/* Jeżeli jesteśmy w liściu to nie ma gdzie zjeżdzać. */
		if (lchild(v) > n) {
			array[v] = value;
			return;
		}

		/* Jeżeli wierzchołek ma jednego syna to sprawa dosyć prosta. */
		if (lchild(v) == n) {
			if (value < array[lchild(v)]) {
				array[v] = value;
			} else {
				array[v] = array[lchild(v)];
				array[lchild(v)] = value;
			}
			return;
		}

		/* Jeżeli ma obu synów to sprawdzamy czy mniejszy zaburzy porządek. */
		vertex m = array[lchild(v)] < array[rchild(v)] ? lchild(v) : rchild(v);
		if (value > array[m]) {
			array[v] = array[m];
			slide_down(m, value);
		} else {
			array[v] = value;
		}
	}

public:
	/**
	 * Tworzy nowy, pusty kopiec o zadanej pojemności.
	 *
	 * @param capacity Maksymalna ilość elementów, którą pomieści kopiec.
	 */
	heap(const uint &capacity) : capacity(capacity), n(0)
	{
		array = new T [capacity + 1];
	}

	/**
	 * Dodaje nowy element do kopca.
	 *
	 * @param element Dodawany element.
	 */
	void push(const T &element)
	{
		n++;
		slide_up(n, element);
	}

	/**
	 * Zwraca najmniejszy element z kopca.
	 *
	 * @return Korzeń kopca.
	 */
	T front() const
	{
		return array[1];
	}

	/**
	 * Usuwa najmniejszy element z kopca i zwraca go.
	 *
	 * @return Korzeń kopca.
	 */
	T pop()
	{
		T result = array[1];
		n--;
		slide_down(1, array[n + 1]);
		return result;
	}

	/**
	 * @return Ilość elementów w kopcu.
	 */
	uint size() const
	{
		return n;
	}

	/**
	 * Sprawdza czy w kopcu są jakieś elementy.
	 *
	 * @return `true` jeżeli nic nie ma, `false` w przeciwnym razie.
	 */
	bool empty() const
	{
		return size() == 0;
	}
};

struct coord {
	uint x, y;

	coord()
	{
	}

	coord(const uint &x, const uint &y) : x(x), y(y)
	{
	}

	uint64 value() const
	{
		return (uint64)x * (uint64)y;
	}

	bool operator <(coord other) const
	{
		return value() > other.value();
	}

	bool operator >(coord other) const
	{
		return value() < other.value();
	}
};

#define INF ((uint64)1 * 'I' * 'N' * 'F' * 'I' * 'N' * 'I' * 'T' * 'Y')

/**
 * Dodaje do kopca potencjalne nowe maksima, usuwając stare.
 */
uint64 repopulate(heap<coord> &h)
{
	const coord current = h.pop();

	/* Idź w górę. */
	if (current.x + 1 == current.y) {
		h.push(coord(current.x, current.y - 1));
	}

	/* Idź w lewo. */
	if (current.x > 1) {
		h.push(coord(current.x - 1, current.y));
	}

	return current.value();
}

int main()
{
	uint n, k;
	scanf("%u %u", &n, &k);

	heap<coord> h(k / 2 + 1);
	h.push(coord(n, n));
	uint64 last = +INF;

	for (uint i = 0; i < k; i++) {
		while (h.front().value() >= last) {
			repopulate(h);
		}
		
		last = repopulate(h);
		printf("%llu\n", last);
	}

	return 0;
}
