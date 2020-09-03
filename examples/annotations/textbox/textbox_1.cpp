#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    plot(iota(1, 10));
    textbox(2, 8, 4, 0.5, "String line from 1 to 10");

    show();
    return 0;
}