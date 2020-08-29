#include <iostream>
#include <thread>
#include <vector>
#include <set>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto [X,Y,Z] = peaks();
    mesh(X,Y,Z);
    zlim({0,inf});

    wait();
    return 0;
}