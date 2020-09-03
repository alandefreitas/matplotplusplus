#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto x = linspace(-5, +5);
    auto y = transform(x, [](double x) { return pow(x, 2); });
    plot(x, y);
    yticks({0, 2, 4, 6, 8, 10, 15, 25});

    show();
    return 0;
}