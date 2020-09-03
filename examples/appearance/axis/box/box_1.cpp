#include <iostream>
#include <matplot/matplot.h>
#include <set>
#include <thread>
#include <vector>

int main() {
    using namespace matplot;

    auto [X, Y, Z] = peaks();
    surf(X, Y, Z);
    box(off);

    show();
    return 0;
}