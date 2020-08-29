#include <matplot/matplot.h>

#include <iostream>
#include <set>
#include <thread>
#include <vector>

int main() {
  using namespace matplot;

  tiledlayout(2, 2);
  auto [x, y, z] = peaks(20);

  nexttile();
  surf(x, y, z);

  nexttile();
  contour(x, y, z);

  nexttile();
  imagesc(z);

  nexttile();
  plot3(x, y, z);

  wait();
  return 0;
}