#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    plot(iota(1, 10));
    textarrow(2.5, 6.5, 5, 5, "y=x");

    show();
    return 0;
}