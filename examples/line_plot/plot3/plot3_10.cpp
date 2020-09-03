#include <cmath>
#include <matplot/matplot.h>
#include <random>

int main() {
    using namespace matplot;

    auto t = iota(0, pi / 500, pi);
    auto xt = transform(t, [](auto t) { return sin(t) * cos(10 * t); });
    auto yt = transform(t, [](auto t) { return sin(t) * sin(10 * t); });
    auto zt = transform(t, [](auto t) { return cos(t); });
    auto p = plot3(xt, yt, zt, "-o");
    p->marker_indices({200});

    show();
    return 0;
}