#include <matplot/matplot.h>

#include <cmath>

int main() {
  using namespace matplot;

  std::vector<double> x = iota(0, 1000);
  std::vector<double> y = transform(x, [](auto x) { return log(x); });
  semilogx(x, y);

  wait();
  return 0;
}