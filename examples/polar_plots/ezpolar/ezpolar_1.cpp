#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    ezpolar("1+cos(t)");

    show();
    return 0;
}