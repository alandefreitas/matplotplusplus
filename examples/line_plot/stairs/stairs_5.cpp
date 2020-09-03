#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<double> x1 = linspace(0, 2 * pi);
    std::vector<double> x2 = linspace(0, pi);
    std::vector<std::vector<double>> X = {x1, x2};

    std::vector<std::vector<double>> Y(2);
    Y[0] = transform(x1, [](double x) { return sin(5 * x); });
    Y[1] = transform(x2, [](double x) { return exp(x) * sin(5 * x); });

    figure();
    stairs(X, Y);

    show();
    return 0;
}