#include <matplot/matplot.h>
#include <random>
#include <tuple>

int main() {
    using namespace matplot;

    std::vector<std::vector<double>> X = {randn(200, 0, 1), randn(200, 0, 1),
                                          randn(200, 0, 1)};
    plotmatrix(X);

    show();
    return 0;
}