#include <matplot/matplot.h>

#include <cmath>

int main() {
  using namespace matplot;

  std::vector<double> theta = linspace(0, 2 * pi);
  std::vector<double> x = transform(theta, [](double t) { return cos(t); });
  std::vector<double> y = transform(theta, [](double t) { return sin(t); });
  std::vector<double> z = theta;

  stem3(x, y, z, ":*m");

  wait();
  return 0;
}