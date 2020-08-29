#include <matplot/matplot.h>

#include <cmath>

int main() {
  using namespace matplot;

  fsurf([](double x, double y) { return erf(x) + cos(y); }, {-5, 0, -5, 5});
  hold(on);
  fsurf([](double x, double y) { return sin(x) + cos(y); }, {0, 5}, {-5, 5});
  hold(off);

  wait();
  return 0;
}