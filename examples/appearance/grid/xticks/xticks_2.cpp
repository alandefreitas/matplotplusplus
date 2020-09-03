#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto x = linspace(-5, +5);
    auto y = transform(x, [](double x) { return pow(x, 2); });
    plot(x, y);
    xticks({-5, -2.5, -1, 0, 1, 2.5, 5});

    show();
    return 0;
}