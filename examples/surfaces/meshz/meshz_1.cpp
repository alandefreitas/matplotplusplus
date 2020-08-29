#include <matplot/matplot.h>

#include <cmath>

int main() {
    using namespace matplot;
    auto [X, Y] = meshgrid(iota(-3, .125, 3));
    auto Z = peaks(X, Y);
    meshz(X, Y, Z);

    wait();
    return 0;
}