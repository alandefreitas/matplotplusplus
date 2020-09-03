#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto x = linspace(0, 1, 100);
    vector_1d y = transform(x, randn(100, 0., 1.),
                            [](double x, double r) { return r * cos(x); });
    scatter(x, y);
    xtickformat("%.2f");

    show();
    return 0;
}