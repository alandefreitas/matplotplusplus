#include <iostream>
#include <matplot/matplot.h>
#include <set>
#include <thread>
#include <vector>

int main() {
    using namespace matplot;

    auto [X, Y, Z] = peaks();
    mesh(X, Y, Z);
    zlim({0, inf});

    show();
    return 0;
}