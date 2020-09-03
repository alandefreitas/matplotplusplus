#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto x = linspace(-pi, pi);
    auto y1 = transform(x, [](double x) { return sin(x); });
    plot(x, y1);

    hold(on);
    auto y2 = transform(x, [](double x) { return cos(x); });
    plot(x, y2);
    hold(off);

    show();
    return 0;
}