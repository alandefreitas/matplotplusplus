#include <matplot/matplot.h>
#include <random>

int main() {
    using namespace matplot;

    auto theta = linspace(0, 1, 500);
    auto x = transform(
        theta, [&](double theta) { return exp(theta) * sin(100 * theta); });
    auto y = transform(
        theta, [&](double theta) { return exp(theta) * cos(100 * theta); });

    auto s = scatter(x, y);
    s->marker_color("b");
    s->marker_face_color({0, .5, .5});

    show();
    return 0;
}