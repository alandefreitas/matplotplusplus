#include <matplot/matplot.h>
#include <random>

int main() {
    using namespace matplot;

    auto theta = linspace(0, 2 * pi, 300);
    auto x =
        transform(theta, [&](double t) { return sin(t) + 0.75 * rand(0, 1); });
    auto y =
        transform(theta, [&](double t) { return cos(t) + 0.75 * rand(0, 1); });
    double sz = 6;

    auto l = scatter(x, y, sz);
    l->marker_color({0, .5, .5});
    l->marker_face_color({0, .7, .7});

    show();
    return 0;
}