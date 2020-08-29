#include <matplot/matplot.h>

#include <cmath>

int main() {
  using namespace matplot;
  auto [X, Y, Z] = peaks();
  mesh(X, Y, Z)->hidden_3d(false);

  wait();
  return 0;
}