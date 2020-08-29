#include <matplot/matplot.h>

#include <cmath>

int main() {
  using namespace matplot;

  fmesh([](double x, double y) { return sin(x) + cos(y); })->edge_color("red");

  wait();
  return 0;
}