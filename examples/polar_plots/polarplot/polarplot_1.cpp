#include <matplot/matplot.h>

#include <cmath>

int main() {
  using namespace matplot;

  std::vector<double> theta = iota(0, 0.01, 2 * pi);
  std::vector<double> rho =
      transform(theta, [](auto t) { return sin(2 * t) * cos(2 * t); });
  polarplot(theta, rho);

  wait();
  return 0;
}