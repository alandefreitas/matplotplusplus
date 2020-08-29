#include <matplot/matplot.h>

#include <cmath>

int main() {
    using namespace matplot;
    std::vector<double> x(40, 1.);
    std::vector<double> y = linspace(-pi / 2, pi / 2, 40);
    auto z = transform(y, [](auto x) { return cos(x); });
    stem3(x, y, z);

    wait();
    return 0;
}