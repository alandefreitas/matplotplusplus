#include <matplot/matplot.h>

#include <cmath>

int main() {
    using namespace matplot;

    plot(iota(1, 10));
    textbox(2, 8, 4, 0.5, "String line from 1 to 10");

    wait();
    return 0;
}