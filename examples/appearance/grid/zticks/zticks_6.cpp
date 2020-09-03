#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto [x, y, z] = peaks();
    mesh(x, y, z);
    zticks({});
    box(off);

    show();
    return 0;
}