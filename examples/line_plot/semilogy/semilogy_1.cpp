#include <matplot/matplot.h>

#include <cmath>

int main() {
  using namespace matplot;

  std::vector<double> x = iota(0, 0.1, 10);
  std::vector<double> y = transform(x, [](auto x) { return exp(x); });
  semilogy(x, y);

  wait();
  return 0;
}