#include <iostream>
#include <thread>
#include <vector>
#include <set>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    tiledlayout(2,1);
    auto ax1 = nexttile();
    auto [x,y,z] = peaks();
    surf(x,y,z);

    auto ax2 = nexttile();
    contour(x,y,z);

    cla(ax1);

    wait();
    return 0;
}