#include <matplot/matplot.h>

#include <cmath>

int main() {
    using namespace matplot;

    std::vector<double> theta = iota(pi / 4, pi / 4, 2 * pi);
    std::vector<double> rho = {19, 6, 12, 18, 16, 11, 15, 15};
    polarscatter(theta, rho);

    wait();
    return 0;
}