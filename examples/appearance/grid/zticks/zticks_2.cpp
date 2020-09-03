#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto [x, y, z] = peaks();
    surf(x, y, z);
    zlim({-10, 10});
    zticks({-10, -2.5, 0, 2.5, 10});

    show();
    return 0;
}