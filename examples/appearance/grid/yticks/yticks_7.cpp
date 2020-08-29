#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    tiledlayout(2, 1);
    auto ax1 = nexttile();
    plot(rand(3, 3, 0, 1));
    auto ax2 = nexttile();
    plot(rand(3, 3, 0, 1));
    yticks(iota(0, .2, 1));

    wait();
    return 0;
}