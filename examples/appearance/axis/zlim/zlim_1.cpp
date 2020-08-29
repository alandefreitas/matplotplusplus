#include <matplot/matplot.h>

#include <iostream>
#include <set>
#include <thread>
#include <vector>

int main() {
  using namespace matplot;

  auto [X, Y, Z] = peaks();
  surf(X, Y, Z);
  zlim({-5, 5});

  wait();
  return 0;
}