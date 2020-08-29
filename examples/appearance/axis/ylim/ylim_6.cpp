#include <matplot/matplot.h>

#include <iostream>
#include <set>
#include <thread>
#include <vector>

int main() {
  using namespace matplot;

  tiledlayout(2, 1);

  auto x = randn(50, 0, 1);
  auto y = randn(50, 0, 1);
  scatter(x, y);

  auto yl = ylim();
  std::cout << "yl[0]: " << yl[0] << std::endl;
  std::cout << "yl[1]: " << yl[1] << std::endl;

  wait();
  return 0;
}