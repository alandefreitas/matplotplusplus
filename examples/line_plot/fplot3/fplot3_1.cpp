#include <matplot/matplot.h>

#include <cmath>

int main() {
  using namespace matplot;

  auto xt = [](double t) { return sin(t); };
  auto yt = [](double t) { return cos(t); };
  auto zt = [](double t) { return t; };
  fplot3(xt, yt, zt);

  wait();
  return 0;
}