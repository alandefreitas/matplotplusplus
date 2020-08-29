#include <iostream>
#include <thread>
#include <vector>
#include <set>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto x = linspace(0,2*pi);
    auto y = transform(x, [](double x) { return sin(x); });
    plot(x,y);
    axis({0,5,-2,2});
    cla();

    wait();
    return 0;
}