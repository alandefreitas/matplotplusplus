#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto x = linspace(0, 50);
    auto y = transform(x, [](double x) { return sin(x / 2.); });
    plot(x, y);
    xticks(iota(0, 10, 50));

    show();
    return 0;
}