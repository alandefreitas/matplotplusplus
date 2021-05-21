#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<double> x = logspace(-1, 2);
    std::vector<double> y1 = transform(x, [](auto x) { return pow(10, x); });
    std::vector<double> y2 =
        transform(x, [](auto x) { return 1. / pow(10, x); });
    auto lg = loglog(x, y1, x, y2);
    lg[0]->line_width(2);
    lg[1]->color({0.4f, 0.f, 1.f});

    show();
    return 0;
}