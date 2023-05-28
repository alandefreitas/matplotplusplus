#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto x = linspace(0, pi);
    auto y1 = transform(x, [](double x) { return cos(x); });
    auto y2 = transform(x, [](double x) { return cos(2 * x); });

    plot(x, y1);
    hold(on);
    plot(x, y2);
    hold(off);

    auto lgd = ::matplot::legend({"cos(x)", "cos(2x)"});
    lgd->location(legend::general_alignment::bottomleft);
    lgd->box(false);

    show();
    return 0;
}