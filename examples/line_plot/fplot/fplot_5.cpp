#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto fp = fplot([](double x) { return sin(x); });
    fp->line_style(":");
    fp->color("r");
    fp->marker("x");
    fp->marker_color("b");

    show();
    return 0;
}