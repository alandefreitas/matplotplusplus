#include <iostream>
#include <thread>
#include <vector>
#include <set>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    tiledlayout(2,1);

    auto ax1 = nexttile();
    plot(ax1, iota(1,10));

    auto ax2 = nexttile();
    plot(ax2, iota(1,10));
    box(ax2,off);

    wait();
    return 0;
}