#include <iostream>
#include <thread>
#include <vector>
#include <set>
#include <matplot/matplot.h>


int main() {
    using namespace matplot;

    auto [X,Y,Z] = peaks();
    surf(X,Y,Z);
    axis(off);

    wait();
    return 0;
}