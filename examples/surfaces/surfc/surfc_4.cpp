#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;
    auto [X, Y] = meshgrid(iota(-5, .5, 5));
    auto Z = transform(
        X, Y, [](double x, double y) { return y * sin(x) - x * cos(y); });

    surfc(X, Y, Z)->edge_color({0, 1, 0, 0});
    colormap({{0, 0, 1}});

    show();
    return 0;
}