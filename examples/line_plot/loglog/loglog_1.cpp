#include <matplot/matplot.h>

#include <cmath>

int main() {
  using namespace matplot;

  std::vector<double> x = logspace(-1, 2);
  std::vector<double> y = transform(x, [](auto x) { return pow(2, x); });
  loglog(x, y);

  wait();
  return 0;
}