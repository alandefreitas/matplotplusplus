#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto [X, Y, Z] = peaks();
    contour(X, Y, Z, 20);

    show();
    return 0;
}