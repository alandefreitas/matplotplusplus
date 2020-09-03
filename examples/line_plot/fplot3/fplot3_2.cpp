#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto xt = [](double t) { return exp(-t / 10.) * sin(5 * t); };
    auto yt = [](double t) { return exp(-t / 10.) * cos(5 * t); };
    auto zt = [](double t) { return t; };
    fplot3(xt, yt, zt, std::array<double, 2>{-10, 10});

    show();
    return 0;
}