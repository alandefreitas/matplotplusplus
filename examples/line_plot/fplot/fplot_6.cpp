#include <matplot/matplot.h>

#include <cmath>

int main() {
  using namespace matplot;

  fplot([](double x) { return sin(x); },
        std::array<double, 2>{-2 * pi, 2 * pi});
  grid(on);
  title("sin(x) from -2π to 2π");
  xlabel("x");
  ylabel("y");

  auto ax = gca();
  ax->x_axis().tick_values(iota(-2 * pi, pi / 2, 2 * pi));
  ax->x_axis().ticklabels(
      {"-2π", "-3π/2", "-π", "-π/2", "0", "π/2", "π", "3π/2", "2π"});

  wait();
  return 0;
}