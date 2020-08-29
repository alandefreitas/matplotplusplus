#include <matplot/matplot.h>

#include <iostream>
#include <set>
#include <thread>
#include <vector>

int main() {
    using namespace matplot;

    auto [x, y, z] = peaks();
    mesh(x, y, z);
    colormap(palette::parula(5));

    wait();
    return 0;
}