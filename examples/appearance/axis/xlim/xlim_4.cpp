#include <iostream>
#include <thread>
#include <vector>
#include <set>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto x = linspace(0,10);
    auto y = transform(x, [](double x) {return sin(x); });
    plot(x,y);

    xlim(manual);
    xlim({0,10});
    hold(on);
    auto x2 = transform(x, [](double x) {return 2*x; });
    auto y2 = transform(y, [](double y) {return 2*y; });
    plot(x2,y2);
    hold(off);

    wait();
    return 0;
}