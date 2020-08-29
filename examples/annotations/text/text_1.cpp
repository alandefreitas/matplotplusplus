#include <matplot/matplot.h>

#include <cmath>

int main() {
  using namespace matplot;

  std::vector<double> x = iota(0, pi / 20, 2 * pi);
  std::vector<double> y = transform(x, [](auto x) { return sin(x); });
  plot(x, y);
  text(pi, 0, "← sin(π)");

  wait();
  return 0;
}