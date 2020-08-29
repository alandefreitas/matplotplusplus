#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto [X, Y, Z] = peaks();
    surf(X, Y, Z);
    grid(off);
    box(off);

    wait();
    return 0;
}