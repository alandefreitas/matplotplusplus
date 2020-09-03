#include <matplot/matplot.h>
#include <thread>

int main() {
    using namespace matplot;

    auto f = [](double x, double y) {
        return erf(pow(y + 2, 3)) -
               exp(-0.65 * (pow(x - 2, 2) + pow(y - 2, 2)));
    };
    fcontour(f)->filled(true).colormap_line_when_filled(true);

    show();
    return 0;
}