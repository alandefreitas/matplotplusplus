#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<double> theta = linspace(0, 2 * pi, 25);
    std::vector<double> rho = transform(theta, [](double t) { return 2 * t; });
    auto p = polarplot(theta, rho);
    p->color("magenta");
    p->marker("square");
    p->marker_size(8.);

    show();
    return 0;
}