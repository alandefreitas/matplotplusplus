#include <matplot/matplot.h>
#include <thread>

int main() {
    using namespace matplot;

    vector_1d x = linspace(-2 * pi, 2 * pi);
    vector_1d y = linspace(0, 4 * pi);
    auto [X, Y] = meshgrid(x, y);
    vector_2d Z =
        transform(X, Y, [](double x, double y) { return sin(x) + cos(y); });
    contour(X, Y, Z);

    show();
    return 0;
}