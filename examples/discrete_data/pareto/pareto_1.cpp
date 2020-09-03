#include <cmath>
#include <matplot/matplot.h>
#include <random>

int main() {
    using namespace matplot;

    std::vector<double> y = {90, 75, 30, 60, 5, 40, 40, 5};
    pareto(y);

    show();
    return 0;
}