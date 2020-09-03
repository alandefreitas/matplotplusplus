#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    tiledlayout(2, 1);
    auto ax1 = nexttile();
    plot(rand(3, 3, 0, 1));
    auto ax2 = nexttile();
    plot(rand(3, 3, 0, 1));
    xticks({1, 2, 3});
    xticklabels({"one", "two", "three"});

    show();
    return 0;
}