#include <iostream>
#include <thread>
#include <vector>
#include <set>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    fplot("sin(x)");
    title("Sine Plot");

    auto ax = gca();
    subplot(2,1,1,ax);

    wait();
    return 0;
}