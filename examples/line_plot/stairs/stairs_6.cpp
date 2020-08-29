#include <matplot/matplot.h>

#include <cmath>

int main() {
  using namespace matplot;

  std::vector<double> X = linspace(0, 4 * pi, 20);
  std::vector<double> Y = transform(X, [](double x) { return sin(x); });

  figure();
  stairs(Y, "-.or");

  wait();
  return 0;
}