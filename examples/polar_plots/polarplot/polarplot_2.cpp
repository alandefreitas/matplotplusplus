#include <matplot/matplot.h>

#include <cmath>

int main() {
  using namespace matplot;

  std::vector<double> theta_degrees = linspace(0, 360, 50);
  std::vector<double> rho =
      transform(theta_degrees, [](auto t) { return 0.005 * t / 10.; });
  std::vector<double> theta_radians = deg2rad(theta_degrees);
  polarplot(theta_radians, rho);

  wait();
  return 0;
}