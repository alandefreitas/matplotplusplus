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

    auto y2 = transform(x, [](double x) { return 2*sin(x); });
    hold(on);
    axis(manual);
    axis({0,10,-1,+1});
    plot(x,y2);
    hold(off);

    axis(automatic);

    wait();
    return 0;
}