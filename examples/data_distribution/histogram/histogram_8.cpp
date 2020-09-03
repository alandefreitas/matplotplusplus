#include <cmath>
#include <matplot/matplot.h>
#include <random>
#include <thread>

int main() {
    using namespace matplot;

    std::vector<double> x = randn(10000, 0, 1);

    auto h = hist(x, histogram::normalization::probability);
    std::cout << "Sum of bar heights: "
              << std::accumulate(h->values().begin(), h->values().end(), 0.0)
              << std::endl;

    show();
    return 0;
}