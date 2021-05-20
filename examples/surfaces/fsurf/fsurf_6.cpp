#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto funx = [](double u, double v) {
        return exp(-std::abs(u) / 10.) * sin(5 * std::abs(v));
    };
    auto funy = [](double u, double v) {
        return exp(-std::abs(u) / 10.) * cos(5 * std::abs(v));
    };
    auto funz = [](double u, double) { return u; };
    fsurf(funx, funy, funz, std::array<double, 2>{-30., +30.})->face_alpha(.5);

    show();
    return 0;
}