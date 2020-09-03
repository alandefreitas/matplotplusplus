#include <iostream>
#include <matplot/matplot.h>
#include <set>
#include <thread>
#include <vector>

int main() {
    using namespace matplot;

    auto [x, y, z] = peaks();

    tiledlayout();
    auto ax1 = nexttile();
    surf(x, y, z);
    colormap(ax1, palette::spring());

    auto ax2 = nexttile();
    surf(x, y, z);
    colormap(ax2, palette::winter());

    show();
    return 0;
}