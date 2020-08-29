#include <iostream>
#include <matplot/matplot.h>
#include <set>
#include <thread>
#include <vector>

int main() {
    using namespace matplot;

    fplot("sin(x)");
    title("Sine Plot");

    auto ax = gca();
    subplot(2, 1, 1, ax);

    wait();
    return 0;
}