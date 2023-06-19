#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;
    auto [X, Y] = meshgrid(iota(-3, .125, 3));
    auto Z = peaks(X, Y);
    auto [FX, FY] = gradient(Z);
    (void) FY;
    meshz(X, Y, Z, FX);
    colorbar();

    show();
    return 0;
}