#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto [x, y, z] = peaks();
    contourf(x, y, z, 20);
    gca()->cb_position({0, 0, 1, 0.06});

    show();
    return 0;
}