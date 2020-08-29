#include <matplot/matplot.h>

#include <cmath>

int main() {
    using namespace matplot;

    plot(iota(1, 10));
    auto [t, a] = textarrow(2.5, 6.5, 5, 5, "y=x");

    wait();
    return 0;
}