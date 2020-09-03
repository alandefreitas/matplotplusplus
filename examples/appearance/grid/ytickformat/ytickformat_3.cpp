#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto x = iota(0, 10);
    vector_1d y = {17, 25, 27, 28, 33, 32, 33, 34, 33, 35};
    plot(x, y, "-V");
    ytickformat("%g M");

    show();
    return 0;
}