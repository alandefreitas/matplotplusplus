#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto r1 = rand(100000, 0, 1);
    auto r2 = rand(100000, 0, 1);
    std::vector<double> theta = transform(
        r1, r2, [](double x, double y) { return atan2(x - .5, 2 * (y - .5)); });
    auto h = polarhistogram(theta, 25);
    h->face_color("red");
    h->face_alpha(1.f - 0.3f);

    show();
    return 0;
}