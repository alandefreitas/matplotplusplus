#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    tiledlayout(2, 1);
    auto ax1 = nexttile();
    auto [X, Y, Z] = peaks(30);
    surf(ax1, X, Y, Z);

    auto ax2 = nexttile();
    auto [X2, Y2, Z2] = peaks(45);
    surf(ax2, X2, Y2, Z2);
    zlabel(ax2, "Height");

    show();
    return 0;
}