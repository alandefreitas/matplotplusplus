#include <iostream>
#include <thread>
#include <vector>
#include <set>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto [X,Y,Z] = peaks();
    surf(X,Y,Z);
    box(on);

    auto ax = gca();
    ax->box_full(true);

    wait();
    return 0;
}