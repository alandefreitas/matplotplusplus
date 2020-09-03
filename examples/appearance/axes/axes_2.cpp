#include <iostream>
#include <matplot/matplot.h>
#include <set>
#include <thread>
#include <vector>

int main() {
    using namespace matplot;
    auto ax1 = axes({0.1, 0.1, 0.7, 0.7});
    auto ax2 = axes({0.65, 0.65, 0.28, 0.28});

    auto [X, Y, Z] = peaks(20);
    contour(ax1, X, Y, Z);
    colorbar(ax1, off);
    surf(ax2, X, Y, Z);

    show();
    return 0;
}