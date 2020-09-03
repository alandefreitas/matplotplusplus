#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto x = linspace(0, 6 * pi);
    auto y = transform(x, [](double x) { return sin(x); });
    plot(x, y);
    xlim({0, 6 * pi});
    yticks(iota(-1, 1, 1));
    yticklabels({"min", "zero", "max"});

    show();
    return 0;
}