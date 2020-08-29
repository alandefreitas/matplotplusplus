#include <matplot/matplot.h>

#include <cmath>

int main() {
  using namespace matplot;

  std::vector<double> rho = iota(10, 5, 70);
  polarplot(rho, "-o");

  wait();
  return 0;
}