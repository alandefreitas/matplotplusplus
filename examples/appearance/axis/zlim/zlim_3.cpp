#include <iostream>
#include <matplot/matplot.h>
#include <set>
#include <thread>
#include <vector>

int main() {
    using namespace matplot;

    auto [X, Y, Z] = peaks();

    tiledlayout(2, 1);

    auto ax1 = nexttile();
    surf(ax1, X, Y, Z);

    auto ax2 = nexttile();
    surf(ax2, X, Y, Z);
    zlim(ax2, {-5, 5});

    show();
    return 0;
}