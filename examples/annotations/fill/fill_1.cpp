#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    vector_1d t = iota(1. / 16, 1. / 8, 1);
    t = transform(t, [](double t) { return t * 2. * pi; });
    vector_1d x = transform(t, [](double t) { return cos(t); });
    vector_1d y = transform(t, [](double t) { return sin(t); });
    std::string color = "r";
    matplot::fill(x, y, color);
    axis(equal);

    show();
    return 0;
}