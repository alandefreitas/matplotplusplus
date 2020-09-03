#include <iostream>
#include <matplot/matplot.h>
#include <set>
#include <thread>
#include <vector>

int main() {
    using namespace matplot;

    auto x = linspace(0, 30);
    auto y1 = transform(x, [](double x) { return sin(x); });
    auto y2 = transform(x, [](double x) { return sin(x / 2); });
    auto y3 = transform(x, [](double x) { return sin(x / 3); });
    auto y4 = transform(x, [](double x) { return sin(x / 4); });

    tiledlayout();
    nexttile();
    plot(x, y1);

    nexttile();
    plot(x, y2);

    nexttile();
    plot(x, y3);

    nexttile();
    plot(x, y4);

    show();
    return 0;
}