#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto ackley = [](double x, double y) {
        return -20 * exp(-0.2 * sqrt(0.5 * (pow(x, 2) + pow(y, 2)))) -
               exp(0.5 * (cos(2 * pi * x) + cos(2 * pi * y))) + exp(1) + 20;
    };
    fcontour(ackley)->n_levels(50).filled(true);

    show();
    return 0;
}