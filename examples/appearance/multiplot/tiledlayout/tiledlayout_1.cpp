#include <iostream>
#include <matplot/matplot.h>
#include <set>
#include <thread>
#include <vector>

int main() {
    using namespace matplot;

    tiledlayout(2, 1);
    auto ax1 = nexttile();
    fplot(ax1, "sin(x)");
    title(ax1, "Plot 1");
    ax1->font_size(14);
    ax1->x_axis().color("red");

    auto ax2 = nexttile();
    fplot(ax2, "cos(x)", "ob");
    title(ax2, "Plot 2");
    grid(ax2, on);

    show();
    return 0;
}