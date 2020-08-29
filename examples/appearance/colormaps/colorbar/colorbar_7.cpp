#include <iostream>
#include <thread>
#include <vector>
#include <set>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto [x,y,z] = peaks();
    surf(x,y,z);
    colorbar(on);
    colorbar(off);

    wait();
    return 0;
}