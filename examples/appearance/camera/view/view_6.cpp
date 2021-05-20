#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto ackley = [](double x, double y) {
        return -20 * exp(-0.2 * sqrt(0.5 * (pow(x, 2) + pow(y, 2)))) -
               exp(0.5 * (cos(2 * pi * x) + cos(2 * pi * y))) + exp(1) + 20;
    };

    gcf()->width(gcf()->width() * 2);
    subplot(1, 2, 0);
    fsurf(ackley)->lighting(true).primary(0.7f).specular(0.9f);
    subplot(1, 2, 1);
    fsurf(ackley);
    rotate(20, 30);

    show();
    return 0;
}