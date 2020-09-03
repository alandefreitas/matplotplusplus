#include <iostream>
#include <matplot/matplot.h>
#include <set>
#include <thread>
#include <vector>

int main() {
    using namespace matplot;

    tiledlayout(2, 1);
    auto ax1 = nexttile();
    auto [x, y, z] = peaks();
    surf(x, y, z);

    auto ax2 = nexttile();
    contour(x, y, z);

    cla(ax1);

    show();
    return 0;
}