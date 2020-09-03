#include <iostream>
#include <matplot/matplot.h>
#include <set>
#include <thread>
#include <vector>

int main() {
    using namespace matplot;

    auto [x, y, z] = peaks();
    mesh(x, y, z);
    colormap(palette::parula(5));

    show();
    return 0;
}