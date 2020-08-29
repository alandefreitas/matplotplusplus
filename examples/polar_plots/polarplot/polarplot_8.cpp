#include <matplot/matplot.h>

#include <cmath>
#include <complex>

int main() {
  using namespace matplot;

  std::vector<std::complex<double>> z = {{2, 3},  {2, 0},   {-1, +4}, {3, -4},
                                         {5, +2}, {-4, -2}, {-2, +3}, {-2, 0},
                                         {0, -3}, {0, 1}};

  polarplot(z);

  wait();
  return 0;
}