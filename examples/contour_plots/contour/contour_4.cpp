#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto [X, Y, Z] = peaks();
    contour(X, Y, Z, "--");

    show();
    return 0;
}