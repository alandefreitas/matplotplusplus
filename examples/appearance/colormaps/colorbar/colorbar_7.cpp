#include <matplot/matplot.h>

#include <iostream>
#include <set>
#include <thread>
#include <vector>

int main() {
  using namespace matplot;

  auto [x, y, z] = peaks();
  surf(x, y, z);
  colorbar(on);
  colorbar(off);

  wait();
  return 0;
}