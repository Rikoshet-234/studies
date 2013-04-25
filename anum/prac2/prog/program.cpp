#include <vector>
#include <cstdio>
#include <cstring>
#include <cmath>

#define ABS(x) ((x) < 0 ? (-(x)) : (x))

/* Aliasy typów w celu ułatwienia sobie życia. */
typedef unsigned int uint;

typedef double real;
typedef std::vector<real> sequence;

const real PI = std::atan(1) * 4;

/**
 * Metoda ewaluacji kombinacji liniowej.
 */
enum method {
  forsythe,
  clenshaw
};

/**
 * Wielomian ortogonalny.
 */
struct orthonomial {

  uint n;
  sequence a, b, c;
  
  orthonomial() {
  }

  orthonomial(const sequence &a, const sequence &b, const sequence &c) : n(c.size() - 1), a(a), b(b), c(c) {    
    /* Ze względów bezpieczeństwa. */
    this->a.push_back(0);
    this->b.push_back(0);
    this->c.push_back(0);
    this->c.push_back(0);
  }

  real eval(const real &x, const uint &k) {
    if (k == 0) {
      return a[0];
    }

    if (k == 1) {
      return (a[1] * x - b[1]) * eval(x, 0);
    }

    real pk, pkm1, pkm2;
    pkm1 = (a[1] * x - b[1]) * eval(x, 0);
    pkm2 = a[0];
    for (uint i = 2; i <= k; i++) {
      pk = (a[i] * x - b[i]) * pkm1 - c[i] * pkm2;
      pkm2 = pkm1;
      pkm1 = pk;
    }

    return pk;
  }

  /**
   * Generuje współczynniki wielomianów Czebyszewa.
   *
   * @param n Jak wiele współczynników wygenerować.
   */
  static orthonomial chebyshev(const uint &n) {
    sequence a(n + 1), b(n + 1), c(n + 1);

    a[0] = 1;
    a[1] = 1; b[1] = 0; c[1] = 0;
    for (uint i = 2; i <= n; i++) {
      a[i] = 2; b[i] = 0; c[i] = 1;
    }

    return orthonomial(a, b, c);
  }
  
  /**
   * Generuje współczynniki wielomianów Hermite'a.
   *
   * @param n Jak wiele współczynników wygenerować.
   */
  static orthonomial hermite(const uint &n) {
    sequence a(n + 1), b(n + 1), c(n + 1);

    a[0] = 1;
    a[1] = 2; b[1] = 0; c[1] = 0;
    for (uint i = 2; i <= n; i++) {
      a[i] = 2; b[i] = 0; c[i] = 2 * (i - 1);
    }

    return orthonomial(a, b, c);
  }
  
  /**
   * Generuje współczynniki wielomianów Lucasa.
   *
   * @param n Jak wiele współczynników wygenerować.
   */
  static orthonomial lucas(const uint &n) {
    sequence a(n + 1), b(n + 1), c(n + 1);

    a[0] = 2;
    a[1] = 0.5; b[1] = 0; c[1] = 0;
    for (uint i = 2; i <= n; i++) {
      a[i] = 1; b[i] = 0; c[i] = -1;
    }

    return orthonomial(a, b, c);
  }

  /**
   * Generuje współczynniki wielomianów Laguerre'a.
   *
   * @param n Jak wiele współczynników wygenerować.
   */
  static orthonomial laguerre(const uint &n) {
    sequence a(n + 1), b(n + 1), c(n + 1);

    a[0] = 1;
    a[1] = -1; b[1] = -1; c[1] = 0;
    for (uint i = 2; i <= n; i++) {
      a[i] = -1.0 / i;
      b[i] = -(2.0 * i - 1.0) / i;
      c[i] = (i - 1.0) / i;
    }

    return orthonomial(a, b, c);
  }

};

/**
 * Kombinacja liniowa wielomianów ortogonalnych.
 */
struct combination {

  orthonomial P;
  sequence w;

  combination() {
  }

  combination(const orthonomial &P, const sequence &w) : P(P), w(w) {
  }

  real eval(const real &x, const method &m, const bool &deriv = false) {
    switch (m) {
    case forsythe:
      return eval_forsythe(x, deriv);
    case clenshaw:
      return eval_clenshaw(x, deriv);
    default:
      return eval_clenshaw(x, deriv);
    }
  }
  /**
   * Oblicza wartość kombinacji liniowej przy użyciu metody Forsythe'a.
   *
   * @param x Punkt w którym wyliczana jest wartość wielomianu.
   * @param deriv Flaga mówiąca czy wyliczana jest pochodna kombinacji.
   */
  real eval_forsythe(const real &x, const bool &deriv = false) {

    if (not deriv) {
      sequence p(P.n + 1);
      p[0] = P.a[0];
      p[1] = (P.a[1] * x - P.b[1]) * p[0];
      
      real s = w[0] * p[0] + w[1] * p[1];
      for (uint i = 2; i <= P.n; i++) {
        p[i] = (P.a[i] * x - P.b[i]) * p[i - 1] - P.c[i] * p[i - 2];
        s += w[i] * p[i];
      }
      
      return s;
    }
    else {
      sequence p(P.n + 1);
      sequence pp(P.n + 1);
      p[0] = P.a[0]; pp[0] = 0;
      p[1] = P.a[1] * x - P.b[1] * p[0]; pp[1] = P.a[1];
      
      real s = w[0] * pp[0] + w[1] * pp[1];
      for (uint i = 2; i <= P.n; i++) {
        p[i] = (P.a[i] * x - P.b[i]) * p[i - 1] - P.c[i] * p[i - 2];
        pp[i] = P.a[i] * p[i - 1] + (P.a[i] * x - P.b[i]) * pp[i - 1] - P.c[i] * pp[i - 2];
        s += w[i] * pp[i];
      }
      
      return s;
    }
  }

  /**
   * Oblicza wartość kombinacji liniowej przy użyciu algorytmu Clenshawa.
   *
   * @param x Punkt w którym wyliczana jest wartość wielomianu.
   * @param deriv Flaga mówiąca czy wyliczana jest pochodna kombinacji.
   */
  real eval_clenshaw(const real &x, const bool &deriv = false) {
 
    sequence B(P.n + 3);

    B[P.n + 2] = B[P.n + 1] = 0;
    for (int k = P.n; k >= 0; k--) {
      B[k] = w[k] + (P.a[k + 1] * x - P.b[k + 1]) * B[k + 1] - P.c[k + 2] * B[k + 2];
    }

    /* Jeżeli nie chcemy liczyć pochodnej to tutaj kończymy. */
    if (not deriv) {
      return P.a[0] * B[0];
    }

    sequence Bp(P.n + 3);

    Bp[P.n + 2] = Bp[P.n + 1] = 0;
    for (int k = P.n; k >= 1; k--) { 
      Bp[k] = (P.a[k] * B[k]) + (P.a[k] * x - P.b[k]) * Bp[k + 1] - P.c[k + 1] * Bp[k + 2];
    }

    return P.a[0] * Bp[1];
  }

  /**
   * Generuje naprzemienną kombinację liniową, tj. P_0 - P_1 + P_2 - P_3...
   *
   * @param P ciąg wielomianów ortogonalnych do wygenerowania kombinacji
   */
  static combination alternate(const orthonomial &P) {
    sequence t(P.n + 1);

    for (uint i = 0; i <= P.n; i++) {
      t[i] = i % 2 == 0 ? 1 : -1;
    }

    return combination(P, t);
  }

  /**
   * Generuje wielomiany I(x) z treści zadania.
   *
   * @param f interpolowana funkcja
   * @param n ilość węzłów interpolacji (stopień wielomianu)
   */
  static combination I(real f(real), const uint &n) {
    sequence t(n + 1);
    for (uint k = 0; k <= n; k++) {
      t[k] = std::cos(PI * (2 * k + 1) / (2 * n + 2));
    }

    sequence *T = new sequence [n + 1];
    for (uint k = 0; k <= n; k++) {
      T[k].push_back(1);
      T[k].push_back(t[k]);

      for (uint j = 2; j <= n; j++) {
        T[k].push_back(2 * t[k] * T[k][j - 1] - T[k][j - 2]);
      }
    }

    /* Współczynniki obliczanej kombinacji. */
    sequence w(n + 1);
    for (uint i = 0; i <= n; i++) {
      w[i] = 0;
      for (uint j = 0; j <= n; j++) {
        w[i] += f(t[j]) * T[j][i];
      }
      w[i] *= (2.0 / (n + 1));

      if (i == 0) {
        w[i] /= 2;
      }
    }

    return combination(orthonomial::chebyshev(n), w);
  }

  /**
   * Generuje wielomiany J(x) z treści zadania.
   *
   * @param f interpolowana funkcja
   * @param n ilość węzłów interpolacji (stopień wielomianu)
   */
  static combination J(real f(real), const uint &n) {
    sequence u(n + 1);
    for (uint k = 0; k <= n; k++) {
      u[k] = std::cos(PI * k / n);
    }

    sequence *T = new sequence [n + 1];
    for (uint k = 0; k <= n; k++) {
      T[k].push_back(1);
      T[k].push_back(u[k]);

      for (uint j = 2; j <= n; j++) {
        T[k].push_back(2 * u[k] * T[k][j - 1] - T[k][j - 2]);
      }   
    }

    /* Współczynniki obliczanej kombinacji. */
    sequence w(n + 1);
    for (uint j = 0; j <= n; j++) {
      w[j] = 0;
      for (uint k = 0; k <= n; k++) {
        real value = f(u[k]) * T[j][k];
        if (k == 0 or k == n) {
          value /= 2;
        }

        w[j] += value;
      }

      w[j] *= (2.0 / n);

      if (j == 0 or j == n) {
        w[j] /= 2;
      }
    }

    return combination(orthonomial::chebyshev(n), w);
  }
};

/* Funkcje do testowania. */
real runge(real x) {
  return 1 / (1 + 25 * x * x);
}

real rungep(real x) {
  const real t = (1 + 25 * x * x);
  return (-50 * x) / (t * t);
}

real simple(real x) {
  return 3 * x * x * x - 2 * x * x + 4 * x;
}

real simplep(real x) {
  return 9 * x * x - 4 * x + 4;
}

int main(int argc, char *argv[]) {
  uint n = 10;
  bool deriv = false;
  method method;
  real (*function)(real);
  real (*functionp)(real);
  combination polynomial;

  bool errormode = false;

  char mtype[100] = "clenshaw";
  char ftype[100] = "null";
  char ptype[100] = "null";

  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "deriv") == 0 or strcmp(argv[i], "d") == 0) {
      deriv = true;
      continue;
    }

    if (strcmp(argv[i], "error") == 0 or strcmp(argv[i], "e") == 0) {
      errormode = true;
      continue;
    }

    bool correct =
      sscanf(argv[i], "n:%u", &n)
    ||
      sscanf(argv[i], "method:%s", mtype)
    ||
      sscanf(argv[i], "m:%s", mtype)
    ||
      sscanf(argv[i], "function:%s", ftype)
    ||
      sscanf(argv[i], "f:%s", ftype)
    ||
      sscanf(argv[i], "polynomial:%s", ptype)
    ||
      sscanf(argv[i], "p:%s", ptype)
    ;

    if (not correct) {
      printf("Inncorrect argument: %s.\n", argv[i]);
      return 0;
    }
  }

  if (strcmp(mtype, "clenshaw") == 0) {
    method = clenshaw;
  }
  else if (strcmp(mtype, "forsythe") == 0 or strcmp(mtype, "standard") == 0) {
    method = forsythe;
  }
  else {
    printf("Unknown evaluationg method: %s.\n", mtype);
  }

  if (strcmp(ftype, "runge") == 0) {
    function = runge;
    functionp = rungep;
  }
  else if (strcmp(ftype, "simple") == 0) {
    function = simple;
    functionp = simplep;
  }
  else if (strcmp(ftype, "exp") == 0) {
    function = exp;
    functionp = exp;
  }
  else if (strcmp(ftype, "sin") == 0) {
    function = sin;
    functionp = cos;
  }
  else {
    printf("Unknown function: %s.\n", ftype);
    return 0;
  }

  if (strcmp(ptype, "I") == 0) {
    polynomial = combination::I(function, n);
  }
  else if (strcmp(ptype, "J") == 0) {
    polynomial = combination::J(function, n);
  }
  else {
    printf("Unknown polynomial type: %s\n", ptype);
    return 0;
  }

  for (real x = -1.0; x <= 1.0; x += 0.001) {
    real value = polynomial.eval(x, method, deriv);

    if (errormode) {
      value = ABS(value - (deriv ? functionp(x) : function(x)));
    }

    printf("%0.16f %0.16f\n", x, value);
  }

  return 0;
}
