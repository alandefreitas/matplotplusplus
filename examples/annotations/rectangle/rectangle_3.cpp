#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    rectangle(2, 4, 2, 2, 1.);
    auto r2 = rectangle(2, 4, 2, 2, 0.);
    r2->color("red");
    axis(equal);

    show();
    return 0;
}