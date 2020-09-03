#include <matplot/matplot.h>
#include <vector>

int main() {
    using namespace matplot;

    tiledlayout();
    auto ax1 = nexttile();
    auto [x, y, z] = peaks();
    contourf(x, y, z);
    colormap(ax1, palette::hot(8));

    auto ax2 = nexttile();
    contourf(x, y, z);
    colormap(ax2, palette::pink());

    show();
    return 0;
}