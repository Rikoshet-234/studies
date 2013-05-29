/*
 * Łukasz Hanuszczak
 * numer indeksu: 247952
 * grupa: KLO
 */

#include <cstdio>
#include <cstring>
 
typedef unsigned int uint;
typedef char nonterm;
typedef char term;
typedef unsigned char set;

struct prod1 {
	nonterm a, b;
};

#define MAX_L 500
#define MAX_M1 (8 * 8 * 8)
#define MAX_M2 (26 * 8)

#define NONTERM(v) (1 << (v - 'A'))
#define TERM(v) (v - 'a')

set prods1[0x100][0x100]; /* Produkcje typu I. */
set prods2[TERM('z') + 1]; /* Produkcje typu II. */

char word[MAX_L + 1]; /* Słowo które chcemy sprawdzić. */

prod1 unique[MAX_M1]; /* Lista unikalnych prawych stron produkcji typu I. */

set cross[0x100][0x100]; /* Z jakich symboli da się dojść do innych. */
set poss[MAX_L + 1][MAX_L + 1]; /* Z jakich symboli można zrobić podsłowo. */

/**
 * Czyści listy produkcji.
 */
inline void clear_prods()
{
	for (set A = NONTERM('A'); A != 0; A <<= 1) {
		for (set B = NONTERM('A'); B != 0; B <<= 1) {
			prods1[A][B] = 0;
		}
	}

	for (term t = 'a'; t <= 'z'; t++) {
		prods2[TERM(t)] = 0;
	}
}

/**
 * Czyści tablicę "dojść".
 */
inline void clear_cross()
{
	for (set A = NONTERM('A'); A != 0; A++) {
		for (set B = NONTERM('A'); B != 0; B++) {
			cross[A][B] = 0;
		}
	}
}

/**
 * Aktualizuje tablicę "dojść" dla danych produkcji o nowy zbiór.
 *
 * @param s Dodatkowy zbiór produkcji.
 * @param a Singleton "lewego" symbolu nieterminalnego.
 * @param b Singleton "prawego" symbolu nieterminalnego.
 */
inline void update_cross(const set &s, const set &a, const set &b)
{
	for (set A = NONTERM('A'); A != 0; A++) {
		for (set B = NONTERM('A'); B != 0; B++) {
			cross[A | a][B | b] |= s;
		}
	}
}

/**
 * Idzie po wszystkich możliwych zbiorach i uzupełnia tablicę "dojść".
 */
inline void fill_cross(const size_t &ucount)
{
	for (uint i = 0; i < ucount; i++) {
		const set a = NONTERM(unique[i].a);
		const set b = NONTERM(unique[i].b);
		update_cross(prods1[a][b], a, b);
	}
}

/**
 * Wylicza zbiór symboli nieterminalnych z których można dojść do danego
 * podsłowa słowa `word`.
 *
 * @param a Indeks początku podsłowa.
 * @param b Indeks końca podsłowa.
 */
inline void eval_poss(const uint &a, const uint &b)
{
	poss[a][b] = 0;
	for (uint i = a; i < b; i++) {
		poss[a][b] |= cross[poss[a][i]][poss[i + 1][b]];
	}
}

/**
 * Korzystając z programowania dynamicznego uzupełnia tablicę `poss`.
 */
inline void fill_poss(const size_t &wlen)
{
	/* Wypełniamy przekątną. */
	for (uint l = 0; l < wlen; l++) {
		poss[l][l] = prods2[TERM(word[l])];
	}

	/* A teraz całą resztę. */
	for (uint a = 1; a < wlen; a++) {
		for (uint b = 0; b < wlen - a; b++) {
			eval_poss(b, b + a);
		}
	}
}

int main()
{
	uint d; /* Ilość przypadków testowy. */
	scanf("%u\n", &d);

	for (uint t = 0; t < d; t++) {
		uint m1, m2; /* Ilośc produkcji odpowiednio typu I i II. */
		scanf("%u %u\n", &m1, &m2);
		
		clear_cross();	
		clear_prods();

		/* Wczytywanie produkcji typu I. */
		size_t ucount = 0;
		for (uint i = 0; i < m1; i++) {
			nonterm s;
			nonterm a, b;
			scanf("%c %c %c\n", &s, &a, &b);

			/* Jeżeli konieczne to dodajemy do listy unikatów. */
			if (prods1[NONTERM(a)][NONTERM(b)] == 0) {
				unique[ucount].a = a;
				unique[ucount].b = b;
				ucount++;
			}
			prods1[NONTERM(a)][NONTERM(b)] |= NONTERM(s);
		}

		/* Wczytywanie produkcji typu II. */
		for (uint i = 0; i < m2; i++) {
			nonterm s;
			term t1;
			scanf("%c %c\n", &s, &t1);
			prods2[TERM(t1)] |= NONTERM(s);
		}

		/* Wczytywanie słowa które będziemy chcieli sprawdzać. */
		scanf("%s\n", word);
		const size_t wlen = strlen(word);

		fill_cross(ucount);
		fill_poss(wlen);

		printf("%s\n", (poss[0][wlen - 1] & NONTERM('A')) ? "TAK" : "NIE");
	}

	return 0;
}
