#include <tuple>
#include <random>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<std::vector<double>> X = {randn(200,0,1), randn(200,0,1), randn(200,0,1)};
    plotmatrix(X);

    wait();
    return 0;
}