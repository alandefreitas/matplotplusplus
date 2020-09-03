#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    rgbplot(palette::parula());
    hold(on);
    colormap(palette::parula());
    colorbar().tick_values({});

    show();
    return 0;
}