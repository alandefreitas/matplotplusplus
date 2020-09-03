#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto x = linspace(0, 6 * pi);
    auto y = transform(x, [](double x) { return sin(x); });
    plot(x, y);
    xlim({0, 6 * pi});
    xticks(iota(0, pi, 6 * pi));
    xticklabels({"0", "π", "2π", "3π", "4π", "5π", "6π"});

    show();
    return 0;
}