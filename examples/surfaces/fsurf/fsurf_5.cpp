#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto funx = [](double u, double v) { return u * sin(v); };
    auto funy = [](double u, double v) { return -u * cos(v); };
    auto funz = [](double, double v) { return v; };
    fsurf(funx, funy, funz, {-5, 5, -5, -2}, "--")->edge_color("g");
    hold(on);

    fsurf(funx, funy, funz, {-5, 5, -2, +2})->edge_color("none");
    hold(off);

    show();
    return 0;
}