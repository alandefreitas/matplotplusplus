#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    fsurf([](double x, double y) { return peaks(x, y); }, {-3, +3})
        ->contour_base(true);

    wait();
    return 0;
}