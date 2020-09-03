#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;
    std::vector<double> x = linspace(-pi / 2, pi / 2, 40);

    std::vector<std::vector<double>> Z(2);
    Z[0] = transform(x, [](auto x) { return sin(x); });
    Z[1] = transform(x, [](auto x) { return cos(x); });

    stem3(Z);

    show();
    return 0;
}