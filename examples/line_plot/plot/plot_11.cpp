#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    double r = 2;
    double xc = 4;
    double yc = 3;
    std::vector<double> theta = linspace(0, 2 * pi);
    std::vector<double> x =
        transform(theta, [=](auto theta) { return r * cos(theta) + xc; });
    std::vector<double> y =
        transform(theta, [=](auto theta) { return r * sin(theta) + yc; });
    plot(x, y);
    axis(equal);

    show();
    return 0;
}