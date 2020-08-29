#include <matplot/matplot.h>

#include <iostream>
#include <set>
#include <thread>
#include <vector>

int main() {
    using namespace matplot;

    auto x = linspace(0, 2 * pi);
    auto y = transform(x, [](double x) { return sin(x); });
    plot(x, y);
    axis({0, 5, -2, 2});
    cla();

    wait();
    return 0;
}