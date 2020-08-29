#include <matplot/matplot.h>

#include <iostream>
#include <set>
#include <thread>
#include <vector>

int main() {
  using namespace matplot;

  auto x = linspace(0, 10);
  auto y = transform(x, [](double x) { return sin(x); });
  plot(x, y);
  xlim({0, 5});

  wait();
  return 0;
}