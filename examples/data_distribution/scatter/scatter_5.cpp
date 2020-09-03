#include <matplot/matplot.h>
#include <random>

int main() {
    using namespace matplot;

    auto theta = linspace(0, 2 * pi, 150);
    auto x =
        transform(theta, [&](double t) { return sin(t) + 0.75 * rand(0, 1); });
    auto y =
        transform(theta, [&](double t) { return cos(t) + 0.75 * rand(0, 1); });
    double sz = 23;

    auto l = scatter(x, y, sz);
    l->marker_style(line_spec::marker_style::diamond);

    show();
    return 0;
}