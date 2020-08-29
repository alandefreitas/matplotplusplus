#include <matplot/matplot.h>

#include <cmath>

int main() {
  using namespace matplot;

  tiledlayout(2, 1);
  auto y1 = rand(3, 3, 0, 1);
  auto ax1 = nexttile();
  plot(y1);

  auto y2 = rand(3, 3, 0, 1);
  auto ax2 = nexttile();
  plot(y2);

  legend(ax1, "Line 1", "Line 2", "Line 3");

  wait();
  return 0;
}