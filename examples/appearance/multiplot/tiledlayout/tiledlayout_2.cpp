#include <iostream>
#include <thread>
#include <vector>
#include <set>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    tiledlayout(2,2);
    auto [x,y,z] = peaks(20);

    nexttile();
    surf(x,y,z);

    nexttile();
    contour(x,y,z);

    nexttile();
    imagesc(z);

    nexttile();
    plot3(x,y,z);

    wait();
    return 0;
}