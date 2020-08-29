#include <matplot/matplot.h>

#include <cmath>

int main() {
  using namespace matplot;

  plot(iota(1, 10));
  auto a = arrow(2.5, 6.5, 5, 5);

  wait();
  return 0;
}