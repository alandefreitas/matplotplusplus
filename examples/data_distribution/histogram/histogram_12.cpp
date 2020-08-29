#include <matplot/matplot.h>

#include <cmath>
#include <random>
#include <thread>

int main() {
  using namespace matplot;

  std::vector<double> x = randn(1000, 0, 1);

  auto h = hist(x);
  title(num2str(h->num_bins()) + " bins");

  for (size_t i = 0; i < 5; ++i) {
    std::this_thread::sleep_for(std::chrono::seconds(3));
    size_t n_bins = fewerbins(h);
    title(num2str(h->num_bins()) + " bins");
  }

  h->num_bins(5);
  title(num2str(h->num_bins()) + " bins");

  wait();
  return 0;
}