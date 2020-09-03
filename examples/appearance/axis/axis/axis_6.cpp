#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto c = eye(10);
    pcolor(c);
    colormap(palette::summer());
    axis(ij);

    show();
    return 0;
}