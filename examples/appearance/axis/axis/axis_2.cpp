#include <matplot/matplot.h>

#include <iostream>
#include <set>
#include <thread>
#include <vector>

int main() {
  using namespace matplot;

  fplot("sin(4*x)/exp(0.1*x)");
  axis({-10, +10, 0, inf});

  wait();
  return 0;
}