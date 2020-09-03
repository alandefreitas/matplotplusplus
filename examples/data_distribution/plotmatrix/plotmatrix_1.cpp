#include <matplot/matplot.h>
#include <random>
#include <tuple>

int main() {
    using namespace matplot;

    std::vector<std::vector<double>> X = {randn(50, 0, 1), randn(50, 0, 1),
                                          randn(50, 0, 1)};
    std::vector<std::vector<double>> Y = {iota(1, 50), iota(51, 100),
                                          iota(101, 150)};
    plotmatrix(X, Y);

    show();
    return 0;
}