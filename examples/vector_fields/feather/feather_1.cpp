#include <matplot/matplot.h>

#include <cmath>

int main() {
    using namespace matplot;
    std::vector<double> theta = iota(-pi / 2, pi / 16, pi / 2);
    std::vector<double> rho(theta.size(), 2.);
    auto [u, v] = pol2cart(theta, rho);
    feather(u, v);

    wait();
    return 0;
}