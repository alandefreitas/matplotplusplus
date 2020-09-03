#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;
    std::vector<double> x = linspace(0, 2 * pi, 50);
    std::vector<double> y =
        transform(x, [](auto x) { return exp(x) * sin(x); });

    stem(x, y, ":dr");

    show();
    return 0;
}