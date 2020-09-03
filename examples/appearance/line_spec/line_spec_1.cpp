#include <iostream>
#include <matplot/matplot.h>
#include <set>
#include <thread>
#include <vector>

int main() {
    using namespace matplot;

    fplot("sin(x)", "-.r*");
    hold(on);
    fplot("sin(x-pi/2)", "--mo");
    fplot("sin(x-pi)", ":bs");

    show();
    return 0;
}