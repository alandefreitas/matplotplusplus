#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;
    auto [X, Y] = meshgrid(iota(-5, .5, +5));
    auto Z = transform(
        X, Y, [](double x, double y) { return y * sin(x) - x * cos(y); });
    mesh(X, Y, Z)->palette_map_at_surface(true).face_alpha(0.5);

    show();
    return 0;
}