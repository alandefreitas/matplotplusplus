#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    vector_1d x = linspace(0, 10);
    auto yl1 = transform(x, [](double x) { return sin(x); });
    auto yl2 = transform(x, [](double x) { return sin(x / 2); });
    plot(x, yl1)->color(gca()->colororder()[0]);
    hold(on);
    plot(x, yl2)->color(gca()->colororder()[0]).line_style("--");

    auto yr1 = x;
    auto yr2 = transform(x, [](double x) { return pow(x, 2); });
    plot(x, yr1)->use_y2(true).color(gca()->colororder()[1]);
    plot(x, yr2)->use_y2(true).color(gca()->colororder()[1]).line_style("--");
    hold(off);

    show();
    return 0;
}