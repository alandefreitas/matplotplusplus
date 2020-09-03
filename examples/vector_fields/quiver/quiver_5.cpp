#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto rastrigin = [](double x, double y) {
        return 10 * 2 + pow(x, 2) - 10 * cos(2 * pi * x) + pow(y, 2) -
               10 * cos(2 * pi * y);
    };

    auto [x, y] = meshgrid(iota(-5, .2, 5));
    auto z = transform(x, y, rastrigin);

    auto [dx, dy] = gradient(z, .2, .2);

    contour(x, y, z);
    hold(on);

    quiver(x, y, dx, dy);
    hold(off);

    show();
    return 0;
}