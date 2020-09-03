#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;
    auto [X, Y] = meshgrid(iota(-3, .125, +3));
    auto Z = peaks(X, Y);
    meshc(X, Y, Z);

    show();
    return 0;
}