#include <matplot/matplot.h>
#include <thread>

int main() {
    using namespace matplot;

    std::vector<double> x = linspace(-2 * pi, 2 * pi);
    std::vector<double> y = linspace(0, 4 * pi);
    auto [X, Y] = meshgrid(x, y);
    vector_2d Z =
        transform(X, Y, [](double x, double y) { return sin(x) + cos(y); });
    contourf(X, Y, Z, 10);

    show();
    return 0;
}