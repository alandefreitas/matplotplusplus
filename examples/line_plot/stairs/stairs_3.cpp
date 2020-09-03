#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto X = linspace(0, 4 * pi, 40);
    auto Y = transform(X, [](double x) { return sin(x); });

    figure();
    stairs(X, Y);

    show();
    return 0;
}