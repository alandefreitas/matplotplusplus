#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto rastrigin = [](double x, double y) {
        return 10 * 2 + pow(x, 2) - 10 * cos(2 * pi * x) + pow(y, 2) -
               10 * cos(2 * pi * y);
    };

    auto [X, Y] = meshgrid(iota(-5, 1., 5), iota(-5, .1, 5));
    auto Z = transform(X, Y, rastrigin);
    ribbon(X, Y, Z)->face_alpha(0.8f);

    show();
    return 0;
}