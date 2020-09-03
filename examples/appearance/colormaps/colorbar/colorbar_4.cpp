#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto [x, y, z] = peaks();
    tiledlayout(2, 1);

    nexttile();
    surf(x, y, z);
    colorbar();

    nexttile();
    mesh(x, y, z);
    colorbar();

    show();
    return 0;
}