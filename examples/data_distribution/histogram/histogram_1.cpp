#include <matplot/matplot.h>

#include <cmath>
#include <random>

int main() {
  using namespace matplot;

  std::vector<double> x = randn(10000, 0, 1);

  auto h = hist(x);
  std::cout << "Histogram with " << h->num_bins() << " bins" << std::endl;

  wait();
  return 0;
}