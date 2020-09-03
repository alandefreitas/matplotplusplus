#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    plot(iota(1, 10));
    text(2, 7, "A simple plot\\nfrom 1 to 10");

    show();
    return 0;
}