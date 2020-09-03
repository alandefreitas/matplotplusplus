#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<double> theta = linspace(0, 360, 50);
    std::vector<double> rho =
        transform(theta, [](double t) { return 0.005 * t / 10.; });
    std::vector<double> th_radians = deg2rad(theta);
    auto s = polarscatter(th_radians, rho);

    show();
    return 0;
}