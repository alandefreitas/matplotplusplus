#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    fmesh([](double x, double y) { return erf(x) + cos(y); },
          std::array<double, 4>{-5, 0, -5, 5});
    hold(on);
    fmesh([](double x, double y) { return sin(x) + cos(y); },
          std::array<double, 2>{0, 5}, std::array<double, 2>{-5, 5});
    hold(off);

    show();
    return 0;
}