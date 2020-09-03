#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    fimplicit([](double x, double y) { return y * sin(x) + x * cos(y); })
        ->color("r")
        .line_style("--")
        .line_width(2);

    show();
    return 0;
}