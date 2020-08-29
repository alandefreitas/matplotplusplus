#include <random>
#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<std::vector<double>> Y = {{2, 2, 2, 2}, {2, 5, 8, 11}, {3, 6, 9, 12}};
    bar(Y);

    wait();
    return 0;
}