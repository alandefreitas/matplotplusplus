#include <matplot/matplot.h>

#include <cmath>

int main() {
    using namespace matplot;

    auto x = linspace(-4, 4);
    auto y = transform(x, [](double x) { return pow(x, 3) - 12 * x; });
    plot(x, y);

    auto e = ellipse(-3.5, 11, 3.5, 8);
    auto r = rectangle(-3.5, 11, 3.5, 8);
    r->color("red");

    wait();
    return 0;
}