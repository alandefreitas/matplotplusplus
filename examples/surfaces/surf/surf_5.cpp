#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;
    auto [X, Y] = meshgrid(iota(1, .5, 10), iota(1, 20));
    auto Z =
        transform(X, Y, [](double x, double y) { return sin(x) + cos(y); });
    surf(X, Y, Z)->lighting(true);

    show();
    return 0;
}