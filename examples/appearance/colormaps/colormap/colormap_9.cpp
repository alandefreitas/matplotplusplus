#include <matplot/matplot.h>
#include <vector>

int main() {
    using namespace matplot;

    auto [X, Y] = meshgrid(iota(-5, .5, 5));
    auto Z = transform(
        X, Y, [](double X, double Y) { return pow(X, 2) + pow(Y, 2); });
    surf(X, Y, Z);
    colorbar();
    caxis({20, 50});

    show();
    return 0;
}