#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto x = iota(0, 0.5, 3);
    auto y = rand(7, 0, 1);
    plot(x, y);
    auto ticks = iota(0, 0.25, 3);
    xticks(ticks);
    xlabel("min");

    show();
    return 0;
}