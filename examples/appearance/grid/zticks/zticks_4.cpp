#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto t = iota(0, pi / 50, 10 * pi);
    auto st = transform(t, [](double t) { return sin(t); });
    auto ct = transform(t, [](double t) { return cos(t); });
    plot3(st, ct, t);
    zlim({0, 40});
    zticks(iota(0, 8, 40));
    zticks(automatic);

    show();
    return 0;
}