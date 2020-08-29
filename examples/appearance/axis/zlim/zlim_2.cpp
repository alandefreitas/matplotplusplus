#include <matplot/matplot.h>

#include <iostream>
#include <set>
#include <thread>
#include <vector>

int main() {
    using namespace matplot;

    auto [X, Y, Z] = peaks();
    mesh(X, Y, Z);
    zlim({0, inf});

    wait();
    return 0;
}