#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto x = iota(0, 10);
    vector_1d y = {.17, .25, .27, .28, .3, .32, .33, .34, .345, .35};
    plot(x, y, "-v");
    xtickformat("%g GHz");

    show();
    return 0;
}