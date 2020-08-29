#include <matplot/matplot.h>

#include <iostream>
#include <set>
#include <thread>
#include <vector>

int main() {
  using namespace matplot;

  auto theta = linspace(0, 2 * pi);
  auto x = transform(theta, [](double theta) { return cos(theta); });
  auto y = transform(theta, [](double theta) { return sin(theta); });
  auto z = theta;
  scatter3(x, y, z);

  zlim(manual);
  zlim({0, 2 * pi});
  hold(on);
  auto z2 = transform(theta, [](double theta) { return 5 * theta; });
  scatter3(x, y, z2);
  hold(off);

  zlim(automatic);

  wait();
  return 0;
}