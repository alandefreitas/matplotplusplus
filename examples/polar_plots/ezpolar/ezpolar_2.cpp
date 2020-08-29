#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    ezpolar([](double t) { return 1. + cos(t); });

    wait();
    return 0;
}