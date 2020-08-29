#include <matplot/matplot.h>

#include <cmath>

int main() {
  using namespace matplot;
  std::vector<double> x = linspace(-pi, pi, 40);
  std::vector<double> z = transform(x, [](double x) { return cos(x); });

  stem3(z, "--*m");

  wait();
  return 0;
}