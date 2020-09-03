#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;
    auto [X, Y] = meshgrid(iota(-5, .5, 5));
    auto Z = transform(
        X, Y, [](double x, double y) { return y * sin(x) - x * cos(y); });
    waterfall(X, Y, Z)->edge_color("b");

    show();
    return 0;
}