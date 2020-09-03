#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto [x, y, z] = peaks();
    surf(x, y, z);
    colorbar();

    show();
    return 0;
}