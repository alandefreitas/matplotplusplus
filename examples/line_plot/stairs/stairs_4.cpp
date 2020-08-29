#include <matplot/matplot.h>

#include <cmath>

int main() {
  using namespace matplot;

  auto X = linspace(0, 4 * pi, 50);
  std::vector<std::vector<double>> Y(2);
  Y[0] = transform(X, [](double x) { return 0.5 * cos(x); });
  Y[1] = transform(X, [](double x) { return 2 * cos(x); });

  figure();
  stairs(X, Y);

  wait();
  return 0;
}