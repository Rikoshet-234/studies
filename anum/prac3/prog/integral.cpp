#include "integral.hpp"

Integral::Integral(
  const std::function<real(real)> &f,
  const real &a, const real &b
) : f(f), a(a), b(b) {
}

real Integral::get_a() {
  return a;
}

real Integral::get_b() {
  return b;
}

void Integral::set_a(const real &a) {
  this->a = a;
}

void Integral::set_b(const real &b) {
  this->b = b;
}

void Integral::set_range(const real &a, const real &b) {
  this->a = a;
  this->b = b;
}


DIntegral::DIntegral(
  const std::function<real(real, real)> &f,
  const real &a, const real &b,
  const real &c, const real &d
) : f(f), a(a), b(b), c(c), d(d) {
}


Romberg::Romberg(
  const std::function<real(real)> &f,
  const real &a, const real &b
) : Integral(f, a, b) {
}

real Romberg::eval() {
  return eval(10);
}

real Romberg::eval(const uint &n) {
  std::vector<std::vector<real>> R(n + 1, std::vector<real>(n + 1));
  real h = b - a;
  R[0][0] = 0.5 * (b - a) * (f(a) + f(b));

  for (uint i = 1; i <= n; i++) {
    h *= 0.5;
    
    real sum = 0.0;
    for (uint k = 1; k <= (1 << (i - 1)); k++) {
      sum += f(a + (2 * k - 1) * h);
    }

    R[i][0] = 0.5 * R[i - 1][0] + h * sum;
    
    for (uint m = 1; m <= i; m++) {
      R[i][m] = R[i][m - 1] + (R[i][m - 1] - R[i - 1][m - 1]) / (std::pow(4, m) - 1);
    }
  }

  return R[n][n];
}


Simpson::Simpson(
  const std::function<real(real)> &f,
  const real &a, const real &b
) : Integral(f, a, b) {
}

real Simpson::eval() {
  return eval(300);
}

real Simpson::eval(const uint &n) {
  real step = (b - a) / n;

  real result = f(a);
  for (real x = a + step; x <= b; x += 2 * step) {
    result += 4 * f(x);
  }
  for (real x = a + 2 * step; x <= b - step; x += 2 * step) {
    result += 2 * f(x);
  }
  result += f(b);
  
  result *= step / 3;

  return result;  
}


Trapezoids::Trapezoids(
  const std::function<real(real)> &f,
  const real &a, const real &b
) : Integral(f, a, b) {
}

real Trapezoids::eval() {
  return eval(1000);
}

real Trapezoids::eval(const uint &n) {
  real step = (b - a) / n;
  
  real result = 0.0;
  
  real prev = f(a);
  for (real x = a + step; x <= b; x += step) {
    const real curr = f(x);
    result += (prev + curr) / 2.0 * step;
    prev = curr;
  }

  return result;
}


Gauss::Gauss(
  const std::function<real(real)> &f,
  const real &a, const real &b
) : Integral(f, a, b) {
}

const uint Gauss::MAX_N = 5;
uint Gauss::N = Gauss::MAX_N;

const std::vector<std::vector<real>> Gauss::X = {
  { 0.50000000000 },
  { 0.2113248654, 0.7886751346 },
  { 0.1127016654, 0.5000000000, 0.8872983346 },
  { 0.0694318442, 0.3300094783, 0.6699905218, 0.9305681558 },
  { 0.0469100771, 0.2307653450, 0.5000000000, 0.7692346551, 0.9530899230 }
};

const std::vector<std::vector<real>> Gauss::W = {
  { 0.6666666667 },
  { 0.5000000000, 0.5000000000 },
  { 0.2777777778, 0.4444444445, 0.2777777778 },
  { 0.1739274226, 0.3260725774, 0.3260725774, 0.1739274226 },
  { 0.1184634425, 0.2393143353, 0.2844444444, 0.2393143353, 0.1184634425 }
};

real Gauss::eval() {
  return eval(100);
}

real Gauss::eval(const uint &k) {
  const real h = (b - a) / k;
  
  real result = 0.0;
  
  real x = a + h;
  for (uint i = 0; i < k; i++) {
    for (uint i = 0; i < N; i++) {
      result += W[N - 1][i] * f(X[N - 1][i] * h - x);
    }
    x += h;
  }

  return result * h;
}
