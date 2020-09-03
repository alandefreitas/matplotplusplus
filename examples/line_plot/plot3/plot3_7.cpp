#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;
    auto t = linspace(-10, 10, 1000);
    auto xt = transform(t, [](auto t) { return exp(-t / 10) * sin(5 * t); });
    auto yt = transform(t, [](auto t) { return exp(-t / 10) * cos(5 * t); });
    auto p = plot3(xt, yt, t);
    p->line_width(3);
    show();
    return 0;
}