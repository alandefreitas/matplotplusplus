#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto x = linspace(0, pi);
    auto y1 = transform(x, [](double x) { return cos(x); });
    auto y2 = transform(x, [](double x) { return cos(2 * x); });
    auto y3 = transform(x, [](double x) { return cos(3 * x); });

    auto p1 = plot(x, y1);
    hold(on);
    plot(x, y2);
    auto p3 = plot(x, y3);
    hold(off);

    ::matplot::legend({p1, p3}, {"First", "Third"});

    show();
    return 0;
}