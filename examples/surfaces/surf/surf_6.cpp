#include <matplot/matplot.h>

#include <cmath>

int main() {
  using namespace matplot;
  auto [X, Y] = meshgrid(iota(1, .5, 10), iota(1, 20));
  auto Z = transform(X, Y, [](double x, double y) { return sin(x) + cos(y); });
  surf(X, Y, Z)->edge_color("none").lighting(true);

  wait();
  return 0;
}