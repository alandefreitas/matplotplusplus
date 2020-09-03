#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto t = iota(0, pi / 10, 10 * pi);
    auto st = transform(t, [](double t) { return sin(t); });
    auto ct = transform(t, [](double t) { return cos(t); });
    plot3(st, ct, t);
    grid(on);
    ztickformat("%g cm");

    show();
    return 0;
}