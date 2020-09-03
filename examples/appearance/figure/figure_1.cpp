#include <iostream>
#include <matplot/matplot.h>
#include <thread>

int main() {
    using namespace matplot;

    auto h = figure();
    auto ax = h->current_axes();
    fplot(ax, "cos(x)");

    show();
    return 0;
}