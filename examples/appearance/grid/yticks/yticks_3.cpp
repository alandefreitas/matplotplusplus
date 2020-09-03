#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto x = linspace(0, 10);
    auto y = transform(x, [](double x) { return pow(x, 2.); });
    plot(x, y);
    yticks(iota(0, 25, 100));

    show();
    return 0;
}