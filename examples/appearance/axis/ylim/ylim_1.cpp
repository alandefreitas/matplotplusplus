#include <iostream>
#include <thread>
#include <vector>
#include <set>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto x = linspace(0,10);
    auto y = transform(x, [](double x) { return sin(x); });
    plot(x,y);
    ylim({-2,+2});

    wait();
    return 0;
}