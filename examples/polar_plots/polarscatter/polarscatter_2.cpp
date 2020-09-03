#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<double> theta = linspace(0, 2 * pi, 20);
    std::vector<double> rho = rand(20, 0, 1);
    double size = 10;
    polarscatter(theta, rho, size, "filled");

    show();
    return 0;
}