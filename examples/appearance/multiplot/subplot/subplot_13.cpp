#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto ax1 = subplot(2, 1, 0);
    auto [x, y, z] = peaks();
    (void) x;
    (void) y;
    z = transpose(z);
    plot(ax1, z);
    xlim(ax1, {0, 20});

    auto ax2 = subplot(2, 1, 1);
    plot(ax2, z);

    ax1->font_size(15);
    ax2->line_width(2);

    show();
    return 0;
}