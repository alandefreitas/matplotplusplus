#include <matplot/matplot.h>

#include <cmath>

int main() {
  using namespace matplot;

  plot(iota(1, 10));
  text({2, 8}, {7, 7},
       std::vector<std::string>({"A simple plot\\nfrom 1 to 10", "y=x"}));

  wait();
  return 0;
}