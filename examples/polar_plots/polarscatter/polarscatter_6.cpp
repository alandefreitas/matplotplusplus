#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<double> theta = iota(pi / 6, pi / 6, 2 * pi);
    std::vector<double> rho1 = rand(12, 0, 1);
    auto ps = polarscatter(theta, rho1, "filled");
    ps->marker("square");
    ps->marker_size(20);
    ps->marker_color({0.5, 1, 0, 0});

    show();
    return 0;
}