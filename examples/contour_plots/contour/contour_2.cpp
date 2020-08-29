#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto [X, Y, Z] = peaks();
    contour(X, Y, Z, 20);

    wait();
    return 0;
}