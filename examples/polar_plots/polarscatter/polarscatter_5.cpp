#include <matplot/matplot.h>

#include <cmath>

int main() {
    using namespace matplot;

    std::vector<double> theta = iota(pi / 6, pi / 6, 2 * pi);
    std::vector<double> rho1 = rand(12, 0, 1);
    polarscatter(theta, rho1, "filled");

    hold(on);
    std::vector<double> rho2 = rand(12, 0, 1);
    polarscatter(theta, rho2, "filled");
    hold(off);

    auto l = legend("Series A", "Series B");
    l->location(legend::general_alignment::topright);

    wait();
    return 0;
}