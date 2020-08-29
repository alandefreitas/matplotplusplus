#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    tiledlayout(2, 1);
    nexttile();
    plot({1., 2., 3.});
    nexttile();
    plot({3., 2., 1.});
    save("img/layout.eps");

    return 0;
}