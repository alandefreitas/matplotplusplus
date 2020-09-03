#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto [x, y, z] = peaks();
    surf(x, y, z);
    colormap(palette::summer());
    colormap(palette::default_map());

    show();
    return 0;
}