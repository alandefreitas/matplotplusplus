#include <matplot/matplot.h>
#include <thread>

int main() {
    using namespace matplot;

    auto rastrigin = [](double x, double y) {
        return 10 * 2 + pow(x, 2) - 10 * cos(2 * pi * x) + pow(y, 2) -
               10 * cos(2 * pi * y);
    };
    fcontour(rastrigin)->filled(true);

    show();
    return 0;
}