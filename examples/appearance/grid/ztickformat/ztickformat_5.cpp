#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    tiledlayout(1, 2);
    auto ax1 = nexttile();
    stem3(ax1, rand(5, 5, 0, 4));
    ztickformat(ax1, "usd");

    auto ax2 = nexttile();
    stem3(ax2, rand(5, 5, 0, 4));

    show();
    return 0;
}
