#include <matplot/matplot.h>

#include <cmath>

int main() {
  using namespace matplot;

  std::vector<double> theta = linspace(0, 2 * pi);
  std::vector<double> rho = transform(theta, [](double t) { return sin(t); });
  polarplot(theta, rho);
  gca()->r_axis().limits({-1, 1});

  wait();
  return 0;
}