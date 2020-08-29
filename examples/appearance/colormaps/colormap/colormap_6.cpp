#include <matplot/matplot.h>

#include <iostream>
#include <set>
#include <thread>
#include <vector>

int main() {
  using namespace matplot;

  std::vector<std::vector<double>> map = {{0, 0, 0.3}, {0, 0, 0.4}, {0, 0, 0.5},
                                          {0, 0, 0.6}, {0, 0, 0.8}, {0, 0, 1}};
  auto [x, y, z] = peaks();
  surf(x, y, z);
  colormap(map);

  wait();
  return 0;
}