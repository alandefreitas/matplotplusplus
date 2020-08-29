#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto x = linspace(0, 10);
    auto y = transform(x, [](double x) { return sin(x); });
    plot(x, y);
    grid(on);
    gca()->minor_grid(true);

    wait();
    return 0;
}