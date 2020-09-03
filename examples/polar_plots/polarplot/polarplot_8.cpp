#include <cmath>
#include <complex>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<std::complex<double>> z = {{2, 3},  {2, 0},   {-1, +4}, {3, -4},
                                           {5, +2}, {-4, -2}, {-2, +3}, {-2, 0},
                                           {0, -3}, {0, 1}};

    polarplot(z);

    show();
    return 0;
}