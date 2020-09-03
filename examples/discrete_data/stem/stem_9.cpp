#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;
    std::vector<double> x = linspace(0, 2 * pi, 50);
    std::vector<double> y =
        transform(x, [](auto x) { return exp(0.3 * x) * sin(3 * x); });

    stem(x, y);
    gca()->x_axis().zero_axis(false);

    show();
    return 0;
}