#include <matplot/matplot.h>

#include <cmath>
#include <random>

int main() {
    using namespace matplot;

    std::vector<double> y = {10, 23, 7, 16, 3};
    pareto(y);

    wait();
    return 0;
}