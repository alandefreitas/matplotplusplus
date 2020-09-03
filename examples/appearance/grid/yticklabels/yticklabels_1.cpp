#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    tiledlayout(2, 1);
    auto ax1 = nexttile();
    plot(rand(3, 3, 0, 1));
    auto ax2 = nexttile();
    plot(rand(3, 3, 0, 1));

    ylim(ax2, {0, 1});
    yticks(ax2, {0, 0.25, .5, .75, 1});
    yticklabels(ax2, {"y=0", "1/4", "1/2", "3/4", "y=1"});

    show();
    return 0;
}