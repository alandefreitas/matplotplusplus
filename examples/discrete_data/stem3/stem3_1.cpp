#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;
    std::vector<double> x = linspace(-pi / 2, pi / 2, 40);
    auto z = transform(x, [](auto x) { return cos(x); });
    stem3(z);

    show();
    return 0;
}