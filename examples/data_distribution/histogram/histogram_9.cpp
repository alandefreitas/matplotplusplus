#include <cmath>
#include <matplot/matplot.h>
#include <random>
#include <thread>

int main() {
    using namespace matplot;

    std::vector<double> x = randn(2000, 0, 1);
    std::vector<double> y = randn(5000, 1, 1);

    auto h1 = hist(x);
    hold(on);
    auto h2 = hist(y);
    h1->normalization(histogram::normalization::probability);
    h1->bin_width(0.25);
    h2->normalization(histogram::normalization::probability);
    h2->bin_width(0.25);

    show();
    return 0;
}