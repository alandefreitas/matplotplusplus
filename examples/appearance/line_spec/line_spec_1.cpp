#include <matplot/matplot.h>

#include <iostream>
#include <set>
#include <thread>
#include <vector>

int main() {
  using namespace matplot;

  fplot("sin(x)", "-.r*");
  hold(on);
  fplot("sin(x-pi/2)", "--mo");
  fplot("sin(x-pi)", ":bs");

  wait();
  return 0;
}