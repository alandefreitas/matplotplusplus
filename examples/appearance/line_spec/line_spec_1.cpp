#include <iostream>
#include <thread>
#include <vector>
#include <set>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    fplot("sin(x)","-.r*");
    hold(on);
    fplot("sin(x-pi/2)","--mo");
    fplot("sin(x-pi)",":bs");

    wait();
    return 0;
}