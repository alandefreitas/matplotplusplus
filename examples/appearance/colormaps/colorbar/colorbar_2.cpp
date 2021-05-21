#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto [x, y, z] = peaks();
    contourf(x, y, z, 20);
    gca()->cb_position({0.f, 0.f, 1.f, 0.06f});

    show();
    return 0;
}