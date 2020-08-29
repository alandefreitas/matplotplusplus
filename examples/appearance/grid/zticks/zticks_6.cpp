#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto [x, y, z] = peaks();
    mesh(x, y, z);
    zticks({});
    box(off);

    wait();
    return 0;
}