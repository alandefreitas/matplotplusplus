#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto x = linspace(0, 10);
    auto y1 = transform(x, [](double x) { return sin(3 * x); });
    plot(x, y1);

    hold(on);
    auto z = transform(x, [](double x) { return sin(3 * x) * exp(0.5 * x); });
    plot(x, z)->use_y2(true);
    y2lim({-150, 150});

    show();
    return 0;
}