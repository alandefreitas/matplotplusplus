#include <matplot/matplot.h>

#include <cmath>

int main() {
  using namespace matplot;
  auto [X, Y] = meshgrid(iota(-3, .5, 3), iota(-3, .1, 3));
  auto Z = peaks(X, Y);
  auto [FX, FY] = gradient(Z);
  ribbon(X, Y, Z, FX, 1.0);
  colorbar();

  wait();
  return 0;
}