#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<double> theta = {0.1, 1.1, 5.4, 3.4, 2.3, 4.5, 3.2,
                                 3.4, 5.6, 2.3, 2.1, 3.5, 0.6, 6.1};
    polarhistogram(theta, 6);

    show();
    return 0;
}