#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;
    auto [X, Y] = meshgrid(iota(1, .5, 10), iota(1, 20));
    auto Z = transform(X, Y, [](double, double) { return 5.; });
    surf(X, Y, Z);

    show();
    return 0;
}