#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto x = linspace(-5, +5);
    auto y = transform(x, [](double x) { return pow(x, 3) - 12 * x; });
    plot(x, y);
    auto t = text({-2, 2}, {16, -16}, "dy/dx=0");
    t->colors({.0, 1.});
    t->sizes({14, 10});
    gca()->colormap({{1, 0, 0}, {0, 0, 0}});

    show();
    return 0;
}