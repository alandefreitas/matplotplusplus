#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    ezpolar("1+cos(t)");

    wait();
    return 0;
}