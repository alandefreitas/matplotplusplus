#include <matplot/matplot.h>

#include <cmath>

int main() {
    using namespace matplot;

    auto funx = [](double u, double v) { return exp(-abs(u) / 10.) * sin(5 * abs(v)); };
    auto funy = [](double u, double v) { return exp(-abs(u) / 10.) * cos(5 * abs(v)); };
    auto funz = [](double u, double v) { return u; };
    fsurf(funx, funy, funz, std::array<double, 2>{-30., +30.})->face_alpha(.5);

    wait();
    return 0;
}