#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    fmesh([](double x, double y) { return sin(x) + cos(y); });

    show();
    return 0;
}