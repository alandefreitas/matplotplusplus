#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    plot(iota(1, 10));
    textarrow(2, 8, 5, 5, "y=x");
    save("img/annotation.eps");

    return 0;
}