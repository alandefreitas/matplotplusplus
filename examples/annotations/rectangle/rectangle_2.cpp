#include <matplot/matplot.h>

#include <cmath>

int main() {
    using namespace matplot;

    rectangle(0, 0, 2, 4, 0);
    rectangle(3, 0, 2, 4, 1);
    rectangle(6, 0, 2, 4, 0.5);

    wait();
    return 0;
}