#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;
    auto [X, Y] = meshgrid(linspace(-5, +5, 70), linspace(-5, +5, 70));
    auto Z = transform(X, Y, [](double x, double y) {
        return 10 * 2 + pow(x, 2) - 10 * cos(2 * pi * x) + pow(y, 2) -
               10 * cos(2 * pi * y);
    });
    surfc(X, Y, Z);

    show();
    return 0;
}