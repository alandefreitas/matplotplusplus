#include <iostream>
#include <thread>
#include <vector>
#include <set>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto [x,y,z] = peaks();
    mesh(x,y,z);
    colormap(palette::parula(5));

    wait();
    return 0;
}