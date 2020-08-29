#include <matplot/matplot.h>

#include <cmath>

int main() {
    using namespace matplot;

    plot(iota(1, 10));
    auto [t, a] = textarrow(2.5, 6.5, 5, 5, "y=x");
    t->color("red").font_size(14);
    a->color("blue");

    wait();
    return 0;
}