#include <matplot/matplot.h>

#include <cmath>

int main() {
  using namespace matplot;

  std::vector<double> x = linspace(0, 2 * pi);
  std::vector<double> y1 = transform(x, [](auto x) { return sin(x); });
  std::vector<double> y2 = transform(x, [](auto x) { return sin(x - 0.25); });
  std::vector<double> y3 = transform(x, [](auto x) { return sin(x - 0.5); });
  plot(x, y1, x, y2, "--", x, y3, ":");

  wait();
  return 0;
}