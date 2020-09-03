#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    vector_1d x = iota(-3, 0.2, 3);
    vector_1d y = iota(-3, 0.2, 3);
    auto [X, Y] = meshgrid(x, y);
    vector_2d Z = transform(X, Y, [](double x, double y) {
        return x * exp(-pow(x, 2.) - pow(y, 2.));
    });
    contour(X, Y, Z)->contour_text(true);

    show();
    return 0;
}