#include <matplot/matplot.h>

#include <cmath>

int main() {
    using namespace matplot;

    auto fp = fplot([](double x) { return sin(x); });
    fp->line_style(":");
    fp->color("r");
    fp->marker("x");
    fp->marker_color("b");

    wait();
    return 0;
}