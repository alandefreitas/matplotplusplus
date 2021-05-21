#include <iostream>
#include <matplot/matplot.h>
#include <set>
#include <thread>
#include <vector>

int main() {
    using namespace matplot;
    auto ax1 = axes({0.1f, 0.1f, 0.7f, 0.7f});
    auto ax2 = axes({0.65f, 0.65f, 0.28f, 0.28f});

    auto [X, Y, Z] = peaks(20);
    contour(ax1, X, Y, Z);
    colorbar(ax1, off);
    surf(ax2, X, Y, Z);

    show();
    return 0;
}