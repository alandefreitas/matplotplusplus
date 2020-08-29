#include <iostream>
#include <thread>
#include <vector>
#include <set>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto [x,y,z] = peaks();

    tiledlayout();
    auto ax1 = nexttile();
    surf(x,y,z);
    colormap(ax1, palette::spring());

    auto ax2 = nexttile();
    surf(x,y,z);
    colormap(ax2, palette::winter());

    wait();
    return 0;
}