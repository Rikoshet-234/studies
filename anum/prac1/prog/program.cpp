#include <iostream>
#include <stdexcept>
#include <vector>

#include <cstring>
#include <cstdio>
#include <cmath>

#define ABS(x) (x < 0 ? -(x) : x)

typedef unsigned int uint;
/* Wybór rodzaju precyzji. */
typedef float real;
/* Kontener do przechowywania ciągów (preferuję wektor niż zwykłą tablicę). */
typedef std::vector<real> sequence;

/* Do porównywania wyników deklaruje stałe z "dość" dużą dokładnością. */
const real PI = 3.1415926535897932384626433832795028841971693993751058;
const real E = 2.7182818284590452353602874713526624977572470936999595;

enum method {
  upstairs,
  downstairs
};

/* Struktura do przechowywania skończonego ułamka łańcuchowego wysokości n. */
struct frac {
  const uint n;
  sequence a;
  sequence b;
  
  frac(const uint &n) : n(n), a(sequence(n + 2)), b(sequence(n + 2)) {
  }

  /* Wylicza wartość ułamka metodą "schodami w górę". */
  real eval_up() {
    real u = b[n];
    for (int k = n - 1; k >= 0; k--) {
      u = a[k + 1] / u + b[k];
    }

    return u;
  }

  /* Wylicza wartość ułamka metodą "schodami w dół". */
  real eval_down() {
    sequence p(n + 2);
    p[0] = 1;
    p[1] = b[0];

    sequence q(n + 2);
    q[0] = 0;
    q[1] = 1;

    for (uint k = 1; k <= n; k++) {
      p[k + 1] = b[k] * p[k] + a[k] * p[k - 1];
      q[k + 1] = b[k] * q[k] + a[k] * q[k - 1];
    }

    return p[n + 1] / q[n + 1];
  }

  real eval(method mtype) {
    switch (mtype) {
    case upstairs:
      return eval_up();
    case downstairs:
      return eval_down(); 
    default:
      throw std::invalid_argument("");
    }
  }

};

/* Ułamek łańcuchowy wysokości n przybliżający 4/π. */
frac four_pi(const uint &n) {
  frac result(n);

  result.b[0] = 1;
  for (uint k = 1; k <= n; k++) {
    result.a[k] = (2 * k - 1) * (2 * k - 1);
    result.b[k] = 2;
  }

  return result;
}

/* Ułamek łańcuchowy wysokości n przybliżający e (z treści zadania). */
frac e(const uint &n) {
  frac result(n);

  result.b[0] = 2;
  for (uint k = 1; k <= n; k++) {
    result.a[k] = result.b[k] = k + 1;
  }

  return result;
}

/* Ułamek łańcuchowy wysokości n przybliżający π. */
frac pi(const uint &n) {
  frac result(n);

  result.b[0] = 3;
  for (uint k = 1; k <= n; k++) {
    /* t to i-ta liczba nieparzysta. */
    const uint t = 2 * k - 1; 
    result.a[k] = t * t;
    result.b[k] = 6;
  }

  return result;
}

/* Ułamek łańcuchowy wysokości n przybliżający π/4 (z treści zadania). */
frac pi_four(const uint &n) {
  frac result(n);

  result.b[0] = 0;
  result.b[1] = 2;
  result.a[1] = 1;
  for (uint k = 2; k <= n; k++) {
    const uint t = 2 * (k - 1) - 1;
    result.a[k] = t * t;
    result.b[k] = 2; 
  }

  return result;
}

/* Ułamek łańcuchowy wysokości n przybliżający wartość arctan w punkcie x. */
frac arctan(const uint &n, const real x) {
  frac result(n);

  result.b[0] = 0;
  result.a[1] = x;
  for (uint k = 1; k <= n; k++) {
    result.b[k] = 2 * k - 1;
    result.a[k + 1] = (k * x) * (k * x);
  }

  return result;
}


int main(int argc, char **argv) {
  uint n = 20;
  char mtype[100] = "null";
  char ftype[100];
  bool baremode = false;
  bool seriesmode = false;
  bool errormode = false;

  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "bare") == 0 || strcmp(argv[i], "b") == 0) {
      baremode = true;
      continue;
    }

    if (strcmp(argv[i], "series") == 0 || strcmp(argv[i], "s") == 0) {
      seriesmode = true;
      continue;
    }

    if (strcmp(argv[i], "error") == 0 || strcmp(argv[i], "e") == 0) {
      errormode = true;
      continue;
    }

    bool correct =
      sscanf(argv[i], "n:%u", &n)
    ||
      sscanf(argv[i], "iterations:%u", &n)
    ||
      sscanf(argv[i], "m:%s", mtype)
    ||
      sscanf(argv[i], "method:%s", mtype)
    ||
      sscanf(argv[i], "f:%s", ftype)
    ||
      sscanf(argv[i], "frac:%s", ftype)
    ;

    if (not correct) {
      printf("Wrong arguments.\n");
      return 0;
    }
  }

  method m;
  if (
    strcmp(mtype, "u") == 0
  ||
    strcmp(mtype, "up") == 0
  ||
    strcmp(mtype, "upstairs") == 0
  ) {
    m = upstairs;
  } else if (
    strcmp(mtype, "d") == 0
  ||
    strcmp(mtype, "down") == 0
  ||
    strcmp(mtype, "downstairs") == 0
  ) {
    m = downstairs;
  } else {
    printf("Unknown method: %s.\n", mtype);
    return 0;
  }
  
  if (not baremode) {
    printf("n    | %s\n", errormode ? "error" : "value");
  }

  for (uint i = (seriesmode ? 1 : n); i <= n; i++) {
    if (not baremode) {
      printf("%04u | ", i);
    }

    if (strcmp("pi", ftype) == 0){
      const real value = pi(i).eval(m);
      printf("%0.16f\n", errormode ? ABS(value - PI) : value);
    }
    else if (strcmp("e", ftype) == 0) {
      const real value = e(i).eval(m);
      printf("%0.16f\n", errormode ? ABS(value - E) : value);
    }
    else if (strcmp("pi_four", ftype) == 0 || strcmp("pi/4", ftype) == 0) {
      const real value = pi_four(i).eval(m);
      printf("%0.16f\n", errormode ? ABS(value - PI / 4) : value);
    }
    else if (strcmp("four_pi", ftype) == 0 || strcmp("4/pi", ftype) == 0) {
      const real value = four_pi(i).eval(m);
      printf("%0.16f\n", errormode ? ABS(value - 4 / PI) : value);
    }
    else if (strcmp("arctan", ftype) == 0 || strcmp("atan", ftype) == 0) {
      if (not baremode and not errormode) {
        printf("x       | arctan(x)\n");
      }

      real totalerror = 0.0;
      uint iters = 0;

      for (real x = -10.0; x <= 10.0; x += 0.001) {
        const real value = arctan(i, x).eval(m);
        if (not errormode) {
          printf("%+0.4f%s%0.16f\n", x, baremode ? " " : " | ", value);
        }
        else {
          totalerror += ABS(value - atan(x));
          iters++;
        }
      }

      if (errormode) {
        printf("%0.16f\n", totalerror / iters);
      }
    }
    else {
      printf("Unknown chained fraction.\n");
      return 0;
    }
  }

  return 0;
}
