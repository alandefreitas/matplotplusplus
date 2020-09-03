#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;
    auto [X, Y] = meshgrid(iota(-3, .5, 3), iota(-3, .1, 3));
    auto Z = peaks(X, Y);
    ribbon(X, Y, Z);

    show();
    return 0;
}