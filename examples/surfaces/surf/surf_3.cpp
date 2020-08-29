#include <matplot/matplot.h>

#include <cmath>

int main() {
    using namespace matplot;
    auto [X, Y] = meshgrid(iota(1, 0.5, 10), iota(1, 20));
    auto Z = transform(X, Y, [](double x, double y) { return sin(x) + cos(y); });
    auto C = transform(X, Y, [](double x, double y) { return x * y; });
    surf(X, Y, Z, C);
    colorbar();

    wait();
    return 0;
}