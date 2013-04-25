#ifndef FILE_FUNCTIONS
#define FILE_FUNCTIONS

#include <functional>
#include <string>
#include <map>
#include <cmath>

#define EPS 0.000000000000001
#define EQUAL(x, y) (std::abs(x - y) < EPS)

typedef unsigned int uint;
typedef double real;

const real PI = 3.1415926535897932385128089594061862044327426701784133911132;

/* Funkcje testowe. */
std::map<std::string, std::function<real(real, real)>> functions = {
  {
    "test1", [](real x, real y) -> real {
      return 1 / (x * x + y * y + 1);
    }
  },
  {
    "test2", [](real x, real y) -> real {
      return x * x - y * y;
    }
  },
  {
    "test3", [](real x, real y) -> real {
      const real dx = (1 - x) * (3 + x);
      const real dy = (1 - y) * (3 + y);
      
      if (EQUAL(dx, 0) || EQUAL(dy, 0)) {
        return 0;
      }

      const real fx = (2 * PI * PI * (1 + x) * (std::sin(PI * (1 + x)))) / dx;
      const real fy = (2 * PI * PI * (1 + y) * (std::sin(PI * (1 + y)))) / dy;

      return fx * fy;
    }
  },
  {
    "sum", [](real x, real y) -> real {
      return x + y;
    }
  },
  {
    "testa1", [](real x, real y) -> real {
      const real xy = x * y;
      
      if (EQUAL(xy, 1)) {
        return 0;
      }
      
      return (x - 1) / ((1 - xy) * log(xy));
    }
  },
  {
    "testa3", [](real x, real y) -> real {
      return 1 / sqrt(1 + x * x + y * y);
    }
  },
  {
    "testa4", [](real x, real y) -> real {
      const real xy = x * y;

      if (EQUAL(xy, 1)) {
        return 0;
      }

      return 1 / (1 - xy);
    }
  }
};

/* Wyniki funkcji testowych, w komentarzu podano przedział całkowania. */
std::map<std::string, real> solutions = {
  { "test1",  /* [ 0; +1] */ 0.639510351870311001962693085427323679 },
  { "test2",  /* [ 0; +1] */ 0 },
  { "test3",  /* [-1; +1] */ 216.88573659103612828913590440710643221049165606488705 },
  { "testa1", /* [ 0; +1] */ 0.5772079363088510328694269 },
  { "testa3", /* [ 0; +1] */ 0.793359308559435594254694 },
  { "testa4", /* [ 0; +1] */ 1.644940990771051492203014 }
};

#endif