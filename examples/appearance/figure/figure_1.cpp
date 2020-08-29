#include <matplot/matplot.h>

#include <iostream>
#include <thread>

int main() {
    using namespace matplot;

    auto h = figure();
    auto ax = h->current_axes();
    fplot(ax, "cos(x)");

    wait();
    return 0;
}