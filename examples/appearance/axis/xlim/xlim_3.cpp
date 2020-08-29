#include <iostream>
#include <thread>
#include <vector>
#include <set>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    tiledlayout(2,1);

    auto x = linspace(0,5,1000);
    auto y = transform(x, [](double x) {return sin(100*x)/exp(x); });

    auto ax1 = nexttile();
    plot(ax1,x,y);

    auto ax2 = nexttile();
    plot(ax2,x,y);
    xlim(ax2,{0,1});

    wait();
    return 0;
}