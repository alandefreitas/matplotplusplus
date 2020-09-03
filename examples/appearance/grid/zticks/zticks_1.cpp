#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto [x, y, z] = peaks();
    surf(x, y, z);
    zlim({-8, 8});
    zticks({-8, 0, +8});
    zticklabels({"z=-8", "z=0", "z=8"});

    show();
    return 0;
}