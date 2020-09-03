#include <matplot/matplot.h>
#include <thread>

int main() {
    using namespace matplot;

    auto rosenbrock = [](double x, double y) {
        return 100 * pow(y - pow(x, 2.), 2.) + pow(1. - x, 2.);
    };
    fcontour(rosenbrock)->n_levels(10).filled(true);

    show();
    return 0;
}