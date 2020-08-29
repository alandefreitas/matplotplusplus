#include <iostream>
#include <thread>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto h = figure();
    auto ax = h->current_axes();
    fplot(ax,"cos(x)");

    wait();
    return 0;
}