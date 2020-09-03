#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto [x, y, z] = peaks();
    surf(x, y, z);
    zlim({-10, 10});
    zticks(iota(-10, 2, 10));

    show();
    return 0;
}