#include <matplot/matplot.h>

#include <cmath>

int main() {
    using namespace matplot;
    auto [X, Y] = meshgrid(iota(-5, 0.5, 5));
    auto Z = transform(X, Y, [](double x, double y) { return y * sin(x) - x * cos(y); });
    auto C = transform(X, Y, [](double x, double y) { return x * y; });
    surf(X, Y, Z)->face_alpha(0.5).edge_color("none");

    wait();
    return 0;
}