#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;
    std::vector<double> x = linspace(0, 10, 20);
    std::vector<double> y = transform(x, [](auto x) { return exp(0.25 * x); });

    stem(x, y, "filled");

    show();
    return 0;
}