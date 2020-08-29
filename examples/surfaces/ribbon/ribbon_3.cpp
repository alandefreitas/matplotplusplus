#include <matplot/matplot.h>

#include <cmath>

int main() {
  using namespace matplot;
  auto [X, Y] = meshgrid(iota(-5, .5, 5));
  auto Z = transform(
      X, Y, [](double x, double y) { return y * sin(x) - x * cos(y); });
  ribbon(X, Y, Z)->edge_color("green").face_alpha(0.5);

  wait();
  return 0;
}