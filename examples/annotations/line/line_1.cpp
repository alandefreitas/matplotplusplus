#include <matplot/matplot.h>

#include <cmath>

int main() {
    using namespace matplot;

    plot(iota(1, 10));

    line(3, 3, 3, 4);
    arrow(3, 4, 4, 4);

    wait();
    return 0;
}