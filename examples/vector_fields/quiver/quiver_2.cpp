#include <matplot/matplot.h>

#include <cmath>

int main() {
  using namespace matplot;
  auto [x, y] = meshgrid(iota(0, 0.2, 2), iota(0, 0.2, 2));
  vector_2d u = transform(x, y, [](double x, double y) { return cos(x) * y; });
  vector_2d v = transform(x, y, [](double x, double y) { return sin(x) * y; });

  quiver(x, y, u, v, 2.);

  wait();
  return 0;
}