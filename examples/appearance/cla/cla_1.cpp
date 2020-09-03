#include <iostream>
#include <matplot/matplot.h>
#include <set>
#include <thread>
#include <vector>

int main() {
    using namespace matplot;

    auto x = linspace(0, 2 * pi);
    auto y1 = transform(x, [](double x) { return sin(x); });
    plot(x, y1);

    hold(on);
    auto y2 = transform(x, [](double x) { return sin(2 * x); });
    plot(x, y2);

    cla();

    auto y3 = transform(x, [](double x) { return sin(3 * x); });
    plot(x, y3);
    hold(off);

    show();
    return 0;
}