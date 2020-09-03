#include <cmath>
#include <matplot/matplot.h>
#include <random>
#include <thread>

int main() {
    using namespace matplot;

    std::vector<double> x = randn(10000, 0, 1);

    std::vector<double> edges = {-10.0000, -2.0000, -1.7500, -1.5000, -1.2500,
                                 -1.0000,  -0.7500, -0.5000, -0.2500, 0,
                                 0.2500,   0.5000,  0.7500,  1.0000,  1.2500,
                                 1.5000,   1.7500,  2.0000,  10.0000};

    hist(x, edges)->normalization(histogram::normalization::count_density);

    show();
    return 0;
}