#include <matplot/matplot.h>

#include <cmath>

int main() {
    using namespace matplot;

    fsurf([](double x, double y) { return sin(x) + cos(y); });

    wait();
    return 0;
}