#include <functional>
#include <stdexcept>
#include <cstdio>
#include <cstring>

#include "functions.hpp"
#include "integral.hpp"

typedef unsigned int uint;
typedef double real;

int main(int argc, char **argv) {
  char fname[100] = "";
  char mname[100] = "";
  
  bool seriesmode = false;
  bool errormode = false;

  uint k = 0;
  uint n = 0;

  real a = 0, b = 1, c = 0, d = 1;

  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "series") == 0) {
      seriesmode = true;
      continue;
    }
    if (strcmp(argv[i], "error") == 0) {
      errormode = true;
      continue;
    }

    bool correct =
      sscanf(argv[i], "function:%s", fname) || sscanf(argv[i], "f:%s", fname)
    ||
      sscanf(argv[i], "precision:%u", &k) || sscanf(argv[i], "k:%u", &k)
    ||
      sscanf(argv[i], "method:%s", mname) || sscanf(argv[i], "m:%s", mname)
    ||
      sscanf(argv[i], "n:%u", &n)
    ||
      sscanf(argv[i], "a:%lf", &a) || sscanf(argv[i], "b:%lf", &b)
    ||
      sscanf(argv[i], "c:%lf", &c) || sscanf(argv[i], "d:%lf", &d)
    ;

    if (not correct) {
      printf("Incorrect argument: %s\n", argv[i]);
      return 0;
    }
  }

  std::function<real(real, real)> func;
  real solut;
  try {
    func = functions.at(fname);
    solut = solutions.at(fname);
  }
  catch (std::out_of_range error) {
    printf("Unknown function: %s\n", fname);
    return 0;
  }

  if (k == 0) {
    printf("Incorrect precision: %u\n", k);
    return 0;
  }

  if (n >= 1 && n <= Gauss::MAX_N) {
    Gauss::N = n;
  }
  else if (n != 0) {
    printf("Incorrect n parameter: %u\n", n);
    return 0;
  }

  DIntegral integral(func, a, b, c, d);

  for (uint t = (seriesmode ? 1 : k); t <= k; t++) {
    real result;
    if (strcmp(mname, "trapezoids") == 0 or strcmp(mname, "t") == 0) {
      result = integral.eval<Trapezoids>(t);
    }
    else if (strcmp(mname, "simpson") == 0 or strcmp(mname, "s") == 0) {
      result = integral.eval<Simpson>(t);
    }
    else if (strcmp(mname, "romberg") == 0 or strcmp(mname, "r") == 0) {
      result = integral.eval<Romberg>(t);
    }
    else if (strcmp(mname, "gauss") == 0 or strcmp(mname, "g") == 0) {
      result = integral.eval<Gauss>(t);
    }
    else {
      printf("Unknown method: %s\n", mname);
      return 0;
    }

    printf("%u %0.16f\n", t, errormode ? std::abs(solut - result) : result);
  }

  return 0;
}