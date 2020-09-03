#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    plot(iota(1, 10));
    auto t = text(2, 8, "A simple plot")->color("red").font_size(14);

    show();
    return 0;
}