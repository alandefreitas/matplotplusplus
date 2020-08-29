#include <iostream>
#include <thread>
#include <vector>
#include <set>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    tiledlayout(2,1);

    auto x = linspace(0,10,1000);
    auto y = transform(x, [](double x) {return sin(10*x)*exp(.5*x); });

    auto ax1 = nexttile();
    plot(ax1,x,y);

    auto ax2 = nexttile();
    plot(ax2,x,y);
    ylim(ax2,{-10,10});

    wait();
    return 0;
}