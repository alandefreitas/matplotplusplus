#include <iostream>
#include <thread>
#include <vector>
#include <set>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto [X,Y,Z] = peaks();
    surf(X,Y,Z);
    zlim({-5,5});

    wait();
    return 0;
}