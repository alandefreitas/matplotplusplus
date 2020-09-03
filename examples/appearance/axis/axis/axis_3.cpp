#include <iostream>
#include <matplot/matplot.h>
#include <set>
#include <thread>
#include <vector>

int main() {
    using namespace matplot;

    tiledlayout(2, 1);

    auto ax1 = nexttile();
    fplot(ax1, "sin(x)");

    auto ax2 = nexttile();
    fplot(ax2, "x < 5 ? sin(x) : -1");

    axis({ax1, ax2}, {0, 10, -1, 1});

    show();
    return 0;
}