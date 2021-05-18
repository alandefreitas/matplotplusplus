#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;
    auto [x, y] = meshgrid(iota(-2, 0.2, 2));
    auto z = transform(x, y, [](double x, double y) {
        return x * exp(-pow(x, 2) - pow(y, 2));
    });
    auto [dx, dy] = gradient(z, .2, .2);

    contour(x, y, z);
    hold(on);

    quiver(x, y, dx, dy);
    hold(off);

    show();
    return 0;
}
