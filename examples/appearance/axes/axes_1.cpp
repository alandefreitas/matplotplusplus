#include <matplot/matplot.h>

int main() {
    using namespace matplot;
    auto ax1 = gca();
    axis({-100, +100, -30, +30});
    fplot(ax1, "3*cos(3*x) + tan(x)");

    show();
    return 0;
}