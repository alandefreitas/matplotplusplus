#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<double> theta = iota(pi / 4, pi / 4, 2 * pi);
    std::vector<double> rho = {19, 6, 12, 18, 16, 11, 15, 15};
    std::vector<double> sizes = {
        6 * 2, 15 * 2, 20 * 2, 3 * 2, 15 * 2, 3 * 2, 6 * 2, 40 * 2,
    };
    std::vector<double> colors = {
        1, 2, 2, 2, 1, 1, 2, 1,
    };
    auto s = polarscatter(theta, rho, sizes, colors, "filled");

    show();
    return 0;
}