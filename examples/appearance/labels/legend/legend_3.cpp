#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto x = linspace(0, pi);
    auto y1 = transform(x, [](double x) { return cos(x); });
    auto y2 = transform(x, [](double x) { return cos(2 * x); });

    auto p1 = plot(x, y1);
    p1->display_name("cos(x)");

    hold(on);
    auto p2 = plot(x, y2);
    p2->display_name("cos(2x)");
    hold(off);

    ::matplot::legend({});

    show();
    return 0;
}