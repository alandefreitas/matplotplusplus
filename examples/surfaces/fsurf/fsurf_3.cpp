#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto r = [](double u, double v) { return 2 + sin(7 * u + 5 * v); };
    auto funx = [r](double u, double v) { return r(u, v) * cos(u) * sin(v); };
    auto funy = [r](double u, double v) { return r(u, v) * sin(u) * sin(v); };
    auto funz = [r](double u, double v) { return r(u, v) * cos(v); };
    fsurf(funx, funy, funz, {0, 2 * pi, 0, pi})->lighting(on);

    show();
    return 0;
}