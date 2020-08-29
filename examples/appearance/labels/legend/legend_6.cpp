#include <matplot/matplot.h>

#include <cmath>

int main() {
  using namespace matplot;

  auto x = linspace(0, pi);
  auto y1 = transform(x, [](double x) { return cos(x); });
  auto y2 = transform(x, [](double x) { return cos(2 * x); });

  plot(x, y1);
  hold(on);
  plot(x, y2);
  hold(off);

  auto lgd = legend("cos(x)", "cos(2x)");
  title(lgd, "My legend title");

  wait();
  return 0;
}