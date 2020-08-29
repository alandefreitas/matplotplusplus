#include <iostream>
#include <thread>
#include <vector>
#include <set>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    tiledlayout(2,1);

    auto ax1 = nexttile();
    fplot(ax1, "sin(x)");

    auto ax2 = nexttile();
    fplot(ax2, "x < 5 ? sin(x) : -1");

    axis({ax1, ax2}, {0,10,-1,1});

    wait();
    return 0;
}