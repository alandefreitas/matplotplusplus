#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    ezpolar([](double t) { return 1. + cos(t); });

    show();
    return 0;
}