#include <matplot/matplot.h>
#include <thread>

int main() {
    using namespace matplot;

    auto [X, Y, Z] = peaks();
    contourf(X, Y, Z, {2, 3})->contour_text(true);

    show();
    return 0;
}