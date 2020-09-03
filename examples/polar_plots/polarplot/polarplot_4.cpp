#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<double> rho = iota(10, 5, 70);
    polarplot(rho, "-o");

    show();
    return 0;
}