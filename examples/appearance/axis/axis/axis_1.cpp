#include <iostream>
#include <matplot/matplot.h>
#include <set>
#include <thread>
#include <vector>

int main() {
    using namespace matplot;

    fplot("sin(x)", "-o");
    axis({0, 2 * pi, -1.15, 1.5});

    show();
    return 0;
}