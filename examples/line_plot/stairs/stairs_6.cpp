#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<double> X = linspace(0, 4 * pi, 20);
    std::vector<double> Y = transform(X, [](double x) { return sin(x); });

    figure();
    stairs(Y, "-.or");

    show();
    return 0;
}