#include <thread>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto [X,Y,Z] = peaks();
    contourf(X,Y,Z, {2,3})->contour_text(true);

    wait();
    return 0;
}