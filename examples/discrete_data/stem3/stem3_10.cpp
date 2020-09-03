#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<double> x = linspace(-2, +2, 50);
    std::vector<double> y = transform(x, [](double x) { return pow(x, 3); });
    std::vector<double> z = transform(x, [](double x) { return exp(x); });

    tiledlayout(2, 1);

    auto ax1 = nexttile();
    stem(ax1, x, z);

    auto ax2 = nexttile();
    stem3(ax2, x, y, z);

    show();
    return 0;
}