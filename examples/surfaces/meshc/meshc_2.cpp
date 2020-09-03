#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;
    auto [X, Y] = meshgrid(iota(-3, .125, +3));
    auto Z = peaks(X, Y);
    auto C = transform(X, Y, [](double x, double y) { return x * y; });
    meshc(X, Y, Z, C);
    colorbar();

    show();
    return 0;
}