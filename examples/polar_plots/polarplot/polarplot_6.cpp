#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<double> theta = linspace(0, 2 * pi, 25);
    std::vector<double> rho = transform(theta, [](double t) { return 2 * t; });
    polarplot(theta, rho, "r-o");

    show();
    return 0;
}