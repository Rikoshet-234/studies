#ifndef FILE_INTEGRAL
#define FILE_INTEGRAL

#include <functional>
#include <vector>
#include <cmath>
#include <cstdio>

typedef unsigned int uint;
typedef double real;

class Integral {
protected:
  const std::function<real(real)> f;
  real a, b;

public:
  Integral(const std::function<real(real)> &f, const real &a, const real &b);
  
  real get_a();
  real get_b();
  void set_a(const real &a);
  void set_b(const real &b);
  void set_range(const real &a, const real &b);

  virtual real eval() = 0;
  virtual real eval(const uint &n) = 0;
};

class DIntegral {
private:
  const std::function<real(real, real)> f;
  real a, b, c, d;

public:
  DIntegral(
    const std::function<real(real, real)> &f,
    const real &a, const real &b,
    const real &c, const real &d
  );

  template <class I>
  real eval(const uint &k);
};

class Romberg : public Integral {
public:
  Romberg(const std::function<real(real)> &f, const real &a, const real &b);

  virtual real eval();
  virtual real eval(const uint &n);
};

class Simpson : public Integral {
public:
  Simpson(const std::function<real(real)> &f, const real &a, const real &b);

  virtual real eval();
  virtual real eval(const uint &n);
};

class Trapezoids : public Integral {
public:
  Trapezoids(const std::function<real(real)> &f, const real &a, const real &b);
  
  virtual real eval();
  virtual real eval(const uint &n);
};

class Gauss : public Integral {
private:
  static const std::vector<std::vector<real>> X;
  static const std::vector<std::vector<real>> W;

public:
  static const uint MAX_N;
  static uint N;

  Gauss(const std::function<real(real)> &f, const real &a, const real &b);

  uint get_n() const;
  void set_n(const uint &n);
  
  virtual real eval();
  virtual real eval(const uint &k);
};


template <class I>
real DIntegral::eval(const uint &k) {
  using std::placeholders::_1;
  
  std::function<real(real)> g = [k, this](real x) -> real {
    real result =  I(std::bind(f, _1, x), c, d).eval(k);
    return result;
  };

  return I(g, a, b).eval(k);
}

#endif