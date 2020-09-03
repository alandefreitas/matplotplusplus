#include <matplot/matplot.h>

int main() {
    using namespace matplot;
    auto x = linspace(-2 * pi, +2 * pi);
    auto y = transform(x, [](double x) { return sin(x); });
    plot(x, y);
    xlabel("-2π ≤ x ≤ 2π");

    show();
    return 0;
}