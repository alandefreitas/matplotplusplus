#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<double> y = {0.001, 0.01, 0.1, 1, 10, 100};
    loglog(y);

    wait();
    return 0;
}