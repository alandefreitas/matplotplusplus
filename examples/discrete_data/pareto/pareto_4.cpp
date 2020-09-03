#include <cmath>
#include <matplot/matplot.h>
#include <random>

int main() {
    using namespace matplot;

    std::vector<double> y = {10, 23, 7, 16, 3};
    pareto(y);

    show();
    return 0;
}