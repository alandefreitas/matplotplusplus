#include <iostream>
#include <matplot/matplot.h>
#include <set>
#include <thread>
#include <vector>

int main() {
    using namespace matplot;
    auto ax1 = axes({0.1f, 0.1f, 0.6f, 0.6f});
    auto ax2 = axes({0.35f, 0.35f, 0.6f, 0.6f});

    ax1->box(true);
    ax2->box(true);

    axes(ax1);
    auto x = linspace(0, 10);
    auto y = transform(x, [](double x) { return sin(x); });
    plot(x, y);

    show();
    return 0;
}