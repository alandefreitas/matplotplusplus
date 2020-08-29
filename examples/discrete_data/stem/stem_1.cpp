#include <matplot/matplot.h>

#include <cmath>

int main() {
    using namespace matplot;
    std::vector<double> x = linspace(0, 2 * pi, 50);

    std::vector<std::vector<double>> Y(2);
    Y[0] = transform(x, [](auto x) { return cos(x); });
    Y[1] = transform(x, [](auto x) { return 0.5 * sin(x); });

    stem(Y, "-o");

    wait();
    return 0;
}