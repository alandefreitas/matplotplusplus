#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;
    auto [X, Y] = meshgrid(iota(-5, .5, +5));
    auto Z = transform(
        X, Y, [](double x, double y) { return y * sin(x) - x * cos(y); });
    auto sc = meshc(X, Y, Z);
    sc->edge_color("r");
    sc->contour_line_spec().color("b");

    show();
    return 0;
}