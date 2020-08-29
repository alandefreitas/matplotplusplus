#include <matplot/matplot.h>

#include <cmath>

int main() {
    using namespace matplot;

    auto X = linspace(0, 4 * pi, 40);
    auto Y = transform(X, [](double x) { return sin(x); });

    figure();
    stairs(X, Y);

    wait();
    return 0;
}