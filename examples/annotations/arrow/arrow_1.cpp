#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    plot(iota(1, 10));
    auto a = arrow(2.5, 6.5, 5, 5);

    show();
    return 0;
}