#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;
    auto [x, y] = meshgrid(iota(-2.5, 0.25, 2.5), iota(-2.5, 0.25, 2.5));
    vector_2d u =
        transform(x, y, [](double x, double y) { return cos(x) * sin(x + y); });
    vector_2d v =
        transform(x, y, [](double x, double y) { return sin(y) * cos(x + y); });

    vector_2d m =
        transform(u, v, [](double u, double v) { return sqrt(u * u + v * v); });

    quiver(x, y, u, v, m, 0.2)->normalize(true).line_width(1.5);

    colormap(palette::jet());
    show();
    return 0;
}
